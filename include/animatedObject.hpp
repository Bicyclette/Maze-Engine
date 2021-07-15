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
	void update(float animationTime);
	float getScaleFactor(float lastTimestamp, float nextTimestamp, float t);
	int getPositionIndex(float t);
	int getRotationIndex(float t);
	int getScaleIndex(float t);
	glm::mat4 interpolatePosition(float t);
	glm::mat4 interpolateRotation(float t);
	glm::mat4 interpolateScaling(float t);

	std::shared_ptr<Joint> joint;
	glm::mat4 localTransform;
	std::vector<PositionKey> pKeys;
	std::vector<RotationKey> rKeys;
	std::vector<ScalingKey> sKeys;
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
		void playAnimation(std::shared_ptr<Animation> & animation);
		void stopAnimation();
		void calculateJointTransform(std::shared_ptr<Joint> joint, glm::mat4 parentTransform);
		std::array<glm::mat4, 50> & getFinalJointTransform();
		std::shared_ptr<Animation> & getCurrentAnimation();

	private:
		std::array<glm::mat4, 50> finalJointTransform;
		std::shared_ptr<Animation> currentAnimation;
		float currentTime;
};

class AnimatedObject : public Object
{
	public:
		AnimatedObject(const std::string & path, glm::mat4 p_model = glm::mat4(1.0f));
		virtual ~AnimatedObject();
		std::vector<std::shared_ptr<Animation>> & getAnimations();
		std::unique_ptr<Animator> & getAnimator();
		virtual void draw(Shader& shader, std::array<glm::mat4, 50> & finalJointTransform, struct IBL_DATA * iblData = nullptr, DRAWING_MODE mode = DRAWING_MODE::SOLID);

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
		std::unique_ptr<Animator> animator;
};

#endif
