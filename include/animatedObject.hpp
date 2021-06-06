#ifndef ANIMATED_OBJECT_HPP
#define ANIMATED_OBJECT_HPP

#include <glm/gtx/quaternion.hpp>
#include <algorithm>
#include <vector>
#include <array>
#include <map>
#include "object.hpp"
#include "joint.hpp"

struct PositionKey
{
	float timestamp; // in ticks
	glm::vec3 position;
};

struct RotationKey
{
	float timestamp; // in ticks
	glm::quat rotation;
};

struct ScalingKey
{
	float timestamp; // in ticks
	glm::vec3 scale;
};

struct JointAnim
{
	std::shared_ptr<Joint> joint;
	glm::mat4 localTransform;
	std::vector<PositionKey> pKeys;
	std::vector<RotationKey> rKeys;
	std::vector<ScalingKey> sKeys;

	void update(float animationTime)
	{
		glm::mat4 position = interpolatePosition(animationTime);
		glm::mat4 rotation = interpolateRotation(animationTime);
		glm::mat4 scale = interpolateScaling(animationTime);
		localTransform = position * rotation * scale;
	}

	float getScaleFactor(float lastTimestamp, float nextTimestamp, float t)
	{
		float midway = t - lastTimestamp;
		float framesDiff = nextTimestamp - lastTimestamp;
		return midway / framesDiff;
	}

	int getPositionIndex(float t)
	{
		for(int i{0}; i < pKeys.size() - 1; ++i)
		{
			if(t < pKeys[i + 1].timestamp)
				return i;
		}
		return -1;
	}
	int getRotationIndex(float t)
	{
		for(int i{0}; i < rKeys.size() - 1; ++i)
		{
			if(t < rKeys[i + 1].timestamp)
				return i;
		}
		return -1;
	}
	int getScaleIndex(float t)
	{
		for(int i{0}; i < sKeys.size() - 1; ++i)
		{
			if(t < sKeys[i + 1].timestamp)
				return i;
		}
		return -1;
	}

	glm::mat4 interpolatePosition(float t)
	{
		if(pKeys.size() == 1)
		{
			return glm::translate(glm::mat4(1.0f), pKeys[0].position);
		}
		int index_left = getPositionIndex(t);
		int index_right = index_left + 1;
		float scale = getScaleFactor(pKeys[index_left].timestamp, pKeys[index_right].timestamp, t);
		glm::vec3 finalPos = glm::mix(pKeys[index_left].position, pKeys[index_right].position, scale);
		return glm::translate(glm::mat4(1.0f), finalPos);
	}

	glm::mat4 interpolateRotation(float t)
	{
		if(rKeys.size() == 1)
		{
			auto rotation = glm::normalize(rKeys[0].rotation);
			return glm::toMat4(rotation);
		}
		int index_left = getRotationIndex(t);
		int index_right = index_left + 1;
		float scale = getScaleFactor(rKeys[index_left].timestamp, rKeys[index_right].timestamp, t);
		glm::quat finalRotation = glm::slerp(rKeys[index_left].rotation, rKeys[index_right].rotation, scale);
		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}

	glm::mat4 interpolateScaling(float t)
	{
		if(sKeys.size() == 1)
		{
			return glm::scale(glm::mat4(1.0f), sKeys[0].scale);
		}
		int index_left = getScaleIndex(t);
		int index_right = index_left + 1;
		float scale = getScaleFactor(sKeys[index_left].timestamp, sKeys[index_right].timestamp, t);
		glm::vec3 finalScale = glm::mix(sKeys[index_left].scale, sKeys[index_right].scale, scale);
		return glm::scale(glm::mat4(1.0f), finalScale);
	}
};

struct Animation
{
	std::string name;
	float duration; // in ticks
	float ticksPerSecond;
	std::vector<std::shared_ptr<struct JointAnim>> jointAnim;
	std::shared_ptr<Joint> rootJoint;
	glm::mat4 globalInverseTransform;
};

class Animator
{
	public:
		Animator();
		Animator(std::shared_ptr<Animation> animation);
		void updateAnimation(float delta);
		void playAnimation(std::shared_ptr<Animation> animation);
		void stopAnimation();
		void calculateJointTransform(std::shared_ptr<Joint> joint, glm::mat4 parentTransform);
		std::array<glm::mat4, 50> & getFinalJointTransform();
		std::shared_ptr<Animation> & getCurrentAnimation();

	private:
		std::array<glm::mat4, 50> finalJointTransform;
		std::shared_ptr<Animation> currentAnimation;
		float currentTime;
		float deltaTime;
};

class AnimatedObject : public Object
{
	public:
		AnimatedObject(const std::string & path, glm::mat4 p_model = glm::mat4(1.0f));
		virtual ~AnimatedObject();
		std::vector<std::shared_ptr<Animation>> & getAnimations();
		virtual void draw(Shader& shader, std::array<glm::mat4, 50> & finalJointTransform, DRAWING_MODE mode = DRAWING_MODE::SOLID);

	private:
		virtual void load(const std::string & path);
		virtual std::shared_ptr<Mesh> getMesh(aiMesh* mesh, const aiScene* scene);
		void loadJointHierarchy(const aiScene * scene);
		aiNode* getRootBone(aiNode * node, std::vector<std::string> & bNames);
		void computeHierarchy(aiNode* node, std::shared_ptr<Joint> joint, std::vector<std::string> & bNames);
		void loadAnimations(aiAnimation** anims, int animCount);

		std::shared_ptr<Joint> rootJoint; // skeleton hierarchy
		std::map<std::string, std::shared_ptr<Joint>> nameJoint; // [joint_name] => [joint_ptr]
		std::map<std::string, glm::mat4> finalJointTransform; // [joint_name] => [matrix4x4]
		std::vector<std::shared_ptr<Animation>> animations;
};

#endif
