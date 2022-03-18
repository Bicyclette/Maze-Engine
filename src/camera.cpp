#include "camera.hpp"

Camera::Camera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 camPos, glm::vec3 camTarget, glm::vec3 camUp, float camFov, float near, float far) :
	orientation(glm::normalize(camTarget - camPos)),
	minDistanceFromCharacter(7.0f),
	maxDistanceFromCharacter(20.0f),
	distanceFromCharacter(minDistanceFromCharacter),
	recenterTarget(glm::vec3(0.0f, 2.5f, 0.0f)),
	lookAbove(glm::vec3(0.0f, 3.0f, 0.0f)),
	minDistanceFromVehicle(20.0f),
	maxDistanceFromVehicle(30.0f),
	distanceFromVehicle(minDistanceFromVehicle),
	m_listener(camPos, std::array<float, 6>{
				orientation.x,
				orientation.y,
				orientation.z,
				camUp.x,
				camUp.y,
				camUp.z
			})
{
	float aspectRatio = static_cast<float>(scrDim.x) / static_cast<float>(scrDim.y);
	camType = type;
	screenDim = scrDim;
	fov = camFov;
	speed = 6.0f;
	position = camPos;
	previousPosition = position;
	target = camTarget;
	glm::vec3 dist = target - position;
	distanceFromTarget = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
	up = glm::normalize(camUp);
	right = glm::normalize(glm::cross(target - position, up));
	upShift = glm::vec3(0.0f);
	rightShift = glm::vec3(0.0f);
	view = glm::lookAt(position, target, up);
    prev_view = view;
	nearPlane = near;
	farPlane = far;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::updateViewMatrix(glm::vec3 vehiclePos, glm::vec3 vehicleDirection, glm::vec3 vehicleUp, float steering, float steeringIncrement, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	previousPosition = position;
	target = vehiclePos + vehicleUp * 2.5f;
	glm::vec3 offset = -(glm::normalize(vehicleDirection) * distanceFromVehicle);
	offset = offset + vehicleUp * 12.0f * (distanceFromVehicle / minDistanceFromVehicle);
	if(steering > steeringIncrement || steering < -steeringIncrement)
		offset = glm::rotate(offset, steering / 3.0f, vehicleUp);
	position = target + offset;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(glm::normalize(target - position), up));
	up = glm::normalize(glm::cross(right, glm::normalize(target - position)));

	if(inputs.test(3)) // mouse scroll
	{
		zoom(mouse, delta);
	}

    prev_view = view;
	view = glm::lookAt(position, target, up);

    // update listener data
	updateListener();
}

void Camera::updateViewMatrix(glm::vec3 characterPos, glm::vec3 characterDirection, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	previousPosition = position;
	target = characterPos + recenterTarget;
	position = target - (glm::normalize(characterDirection) * distanceFromCharacter) + lookAbove * (distanceFromCharacter / minDistanceFromCharacter);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(glm::normalize(target - position), up));
	up = glm::normalize(glm::cross(right, glm::normalize(target - position)));

	if(inputs.test(3)) // mouse scroll
	{
		zoom(mouse, delta);
	}

    prev_view = view;
	view = glm::lookAt(position, target, up);
	
    // update listener data
	updateListener();
}

void Camera::updateViewMatrix(const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	previousPosition = position;
	if(inputs.test(1) && !inputs.test(4)) // middle mouse button
	{
		tumble(mouse);
	}

	if(inputs.test(1) && inputs.test(4)) // middle mouse button and shift key
	{
		track(mouse, delta);
	}

	if(inputs.test(3)) // mouse scroll
	{
		zoom(mouse, delta);
	}

    prev_view = view;
	view = glm::lookAt(position, target, up);

	// update listener data
	updateListener();
}

void Camera::updateProjectionMatrix(int w, int h)
{
	screenDim.x = w;
	screenDim.y = h;
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
	if(camType == CAM_TYPE::REGULAR && (distanceFromTarget > 0.1f || m[2] < 0))
	{
		position += glm::normalize(target - position) * (m[2] * delta * (speed * 10.0f));
		glm::vec3 front = target - position;
		distanceFromTarget = sqrt(front.x * front.x + front.y * front.y + front.z * front.z);
	}
	else if(camType == CAM_TYPE::THIRD_PERSON)
	{
		float factor = m[2] * delta * (speed * 10.0f);
		distanceFromCharacter -= factor;
		distanceFromCharacter = (distanceFromCharacter < minDistanceFromCharacter) ? minDistanceFromCharacter : distanceFromCharacter;
		distanceFromCharacter = (distanceFromCharacter > maxDistanceFromCharacter) ? maxDistanceFromCharacter : distanceFromCharacter;
	}
	else if(camType == CAM_TYPE::VEHICLE)
	{
		float factor = m[2] * delta * (speed * 20.0f);
		distanceFromVehicle -= factor;
		distanceFromVehicle = (distanceFromVehicle < minDistanceFromVehicle) ? minDistanceFromVehicle : distanceFromVehicle;
		distanceFromVehicle = (distanceFromVehicle > maxDistanceFromVehicle) ? maxDistanceFromVehicle : distanceFromVehicle;
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

glm::mat4 & Camera::getPreviousViewMatrix()
{
	return prev_view;
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

glm::vec3 Camera::getRight()
{
	return right;
}

glm::vec3 Camera::getUp()
{
	return up;
}

glm::vec3 Camera::getOrientation()
{
	return orientation;
}

void Camera::setProjection(glm::ivec2 scrDim, float near, float far)
{
	screenDim = scrDim;
	float aspectRatio = static_cast<float>(screenDim.x) / static_cast<float>(screenDim.y);
	nearPlane = near;
	farPlane = far;
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

CAM_TYPE Camera::getType()
{
	return camType;
}

void Camera::updateListener()
{
	orientation = glm::normalize(target - position);
	m_listener.set_position(position);
	m_listener.set_orientation(std::array<float, 6>{
				orientation.x,
				orientation.y,
				orientation.z,
				up.x,
				up.y,
				up.z
				});
	m_listener.set_velocity(position - previousPosition);
}

void Camera::rotateAroundAxis(glm::vec3 axis, float delta)
{
    glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(delta), axis);
    position = glm::vec3(rot_mat * glm::vec4(position, 1.0f));
    up = glm::vec3(rot_mat * glm::vec4(up, 1.0f));
    up = glm::normalize(up);
	right = glm::cross(glm::normalize(target - position), up);
}
