#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <array>
#include <bitset>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera
{
	public:

		Camera(float aspectRatio, glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f), float camFov = 45.0f, float near = 0.1f, float far = 100.0f);
		void updateViewMatrix(const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		void updateProjectionMatrix(int w, int h);
		glm::mat4 & getViewMatrix();
		glm::mat4 & getProjectionMatrix();
		float getNearPlane();
		float getFarPlane();
		float getFov();
		glm::vec3 getPosition();
		void setProjection(float aspectRatio, float near, float far);

	private:

		glm::quat getRotationQuaternion(int x, int y);
		void tumble(const std::array<int, 3> & m);
		void zoom(const std::array<int, 3> & m, float delta);
		void track(const std::array<int, 3> & m, float delta);

		float fov;
		float speed;
		float yaw;
		float pitch;
		float roll;
		float distanceFromTarget;
		
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 up;
		glm::vec3 right;

		glm::vec3 upShift;
		glm::vec3 rightShift;

		glm::mat4 view;
		glm::mat4 projection;
		float nearPlane;
		float farPlane;
};

#endif
