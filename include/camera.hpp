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
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "audio.hpp"

enum class CAM_TYPE
{
	REGULAR,
	THIRD_PERSON,
	VEHICLE
};

class Camera
{
	public:

		Camera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f), float camFov = 45.0f, float near = 0.1f, float far = 100.0f);
		void updateViewMatrix(const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		void updateViewMatrix(glm::vec3 characterPos, glm::vec3 characterDirection, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		void updateViewMatrix(glm::vec3 vehiclePos, glm::vec3 vehicleDirection, glm::vec3 vehicleUp, float steering, float steeringIncrement, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		void updateProjectionMatrix(int w, int h);
		glm::mat4 & getViewMatrix();
		glm::mat4 & getPreviousViewMatrix();
		glm::mat4 & getProjectionMatrix();
		float getNearPlane();
		float getFarPlane();
		float getFov();
		glm::vec3 getPosition();
		glm::vec3 getRight();
		glm::vec3 getUp();
		glm::vec3 getOrientation();
		void setProjection(glm::ivec2 scrDim, float near, float far);
		CAM_TYPE getType();
        void rotateAroundAxis(glm::vec3 axis, float delta);

	private:

		glm::quat getRotationQuaternion(int x, int y);
		void tumble(const std::array<int, 3> & m);
		void zoom(const std::array<int, 3> & m, float delta);
		void track(const std::array<int, 3> & m, float delta);
		void updateListener();

		CAM_TYPE camType;

		float fov;
		float speed;
		float distanceFromTarget;

		glm::vec3 previousPosition;
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 orientation;
		glm::vec3 up;
		glm::vec3 right;

		glm::vec3 upShift;
		glm::vec3 rightShift;

		Listener m_listener;

		glm::ivec2 screenDim;
		glm::mat4 prev_view;
		glm::mat4 view;
		glm::mat4 projection;
		float nearPlane;
		float farPlane;

	private: // variables for third person camera

		const float maxDistanceFromCharacter;
		const float minDistanceFromCharacter;
		float distanceFromCharacter;
		glm::vec3 recenterTarget;
		glm::vec3 lookAbove;
	
	private: // variables for vehicle camera

		const float maxDistanceFromVehicle;
		const float minDistanceFromVehicle;
		float distanceFromVehicle;
};

#endif
