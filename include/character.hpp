#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <utility>
#include "camera.hpp"
#include "animatedObject.hpp"
#include "shader_light.hpp"

class Character
{
	public:

	enum class DIRECTION
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};

	public:
		Character(
				std::string aPath,
				glm::mat4 aModel = glm::mat4(1.0f),
				std::string aName = "name");
		void walk();
		void run();
		void jump();
		void idle();
		glm::mat4 getModel();
		void setModel(glm::mat4 & m);
		void draw(Shader& shader, struct IBL_DATA * iblData = nullptr, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		std::unique_ptr<AnimatedObject> & get();
		std::shared_ptr<Camera> & getCamera();
		glm::vec3 getPosition();
		glm::vec3 getDirection();

	private:
		std::unique_ptr<AnimatedObject> character;
		std::shared_ptr<Camera> camera;

	public:
		int sceneID;
		std::string name;
		glm::vec3 walkDirection;
		float turnAngle;
		float turnSpeed;
		float walkSpeed;
		float runSpeed;
};

#endif
