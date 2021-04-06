#include "camera.hpp"

Camera::Camera(float aspectRatio, glm::vec3 camPos, glm::vec3 camTarget, glm::vec3 camUp, float camFov, float near, float far)
{
	fov = camFov;
	speed = 6.0f;
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	distanceFromTarget = sqrt(camPos.x * camPos.x + camPos.y * camPos.y + camPos.z * camPos.z);
	position = camPos;
	target = camTarget;
	up = glm::normalize(camUp);
	right = glm::normalize(glm::cross(target - position, up));
	upShift = glm::vec3(0.0f);
	rightShift = glm::vec3(0.0f);
	view = glm::lookAt(position, target, up);
	nearPlane = near;
	farPlane = far;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::updateViewMatrix(const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{

	if(inputs.test(0)) // right mouse button
	{
		
	}

	if(inputs.test(1) && !inputs.test(4)) // middle mouse button
	{
		tumble(mouse);
	}

	if(inputs.test(1) && inputs.test(4)) // middle mouse button and shift key
	{
		track(mouse, delta);
	}

	if(inputs.test(2)) // left mouse button
	{
		
	}

	if(inputs.test(3)) // mouse scroll
	{
		zoom(mouse, delta);
	}

	view = glm::lookAt(position, target, up);
}

void Camera::updateProjectionMatrix(int w, int h)
{
	float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::quat Camera::getRotationQuaternion(int x, int y)
{
	float thetaP = glm::radians(static_cast<float>(-y))/2.0f;
	float thetaY = glm::radians(static_cast<float>(-x))/2.0f;

	// pitch thanks to camera's right axis
	glm::quat rotP = glm::quat(cos(thetaP), right.x * sin(thetaP), right.y * sin(thetaP), right.z * sin(thetaP));

	// yaw thanks to camera's up axis
	glm::quat rotY = glm::quat(cos(thetaY), up.x * sin(thetaY), up.y * sin(thetaY), up.z * sin(thetaY));

	// combine
	glm::quat res = rotY * rotP;

	return res;
}

void Camera::tumble(const std::array<int, 3> & m)
{
	yaw += m[0];
	pitch += m[1];
	if(yaw >= 360.0f)
		yaw = 360.0f - yaw;
	if(pitch >= 360.0f)
		pitch = 360.0f - pitch;
	if(yaw <= -360.0f)
		yaw = 360.0f + yaw;
	if(pitch <= -360.0f)
		pitch = 360.0f + pitch;

	glm::quat rotQuat = getRotationQuaternion(m[0], m[1]);
	glm::quat rotQuatConj = glm::conjugate(rotQuat);

	// apply on camera's position
	glm::vec3 unshiftedPosition = position - upShift - rightShift;
	glm::quat posRot = rotQuat * glm::quat(0.0f, unshiftedPosition.x, unshiftedPosition.y, unshiftedPosition.z) * rotQuatConj;
	position = glm::vec3(posRot.x, posRot.y, posRot.z) + upShift + rightShift;
	
	// apply on camera's up axis
	glm::quat upRot = rotQuat * glm::quat(0.0f, up.x, up.y, up.z) * rotQuatConj;
	up = glm::normalize(glm::vec3(upRot.x, upRot.y, upRot.z));

	// compute camera's right axis
	right = glm::cross(glm::normalize(target - position), up);
}

void Camera::zoom(const std::array<int, 3> & m, float delta)
{
	if(distanceFromTarget > 0.1f || m[2] < 0)
	{
		position += glm::normalize(target - position) * (m[2] * delta * (speed * 10.0f));
		glm::vec3 front = target - position;
		distanceFromTarget = sqrt(front.x * front.x + front.y * front.y + front.z * front.z);
	}
}

void Camera::track(const std::array<int, 3> & m, float delta)
{
	position -= static_cast<float>(m[0]) * right * delta * speed;
	target -= static_cast<float>(m[0]) * right * delta * speed;
	rightShift -= static_cast<float>(m[0]) * right * delta * speed;
	
	position += static_cast<float>(m[1]) * up * delta * speed;
	target += static_cast<float>(m[1]) * up * delta * speed;
	upShift += static_cast<float>(m[1]) * up * delta * speed;
}

glm::mat4 & Camera::getViewMatrix()
{
	return view;
}

glm::mat4 & Camera::getProjectionMatrix()
{
	return projection;
}

float Camera::getNearPlane()
{
	return nearPlane;
}

float Camera::getFarPlane()
{
	return farPlane;
}

float Camera::getFov()
{
	return fov;
}

glm::vec3 Camera::getPosition()
{
	return position;
}

void Camera::setProjection(float aspectRatio, float near, float far)
{
	nearPlane = near;
	farPlane = far;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
