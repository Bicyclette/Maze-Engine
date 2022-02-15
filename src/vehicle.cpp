#include "vehicle.hpp"

Vehicle::Vehicle(
		std::array<float, 6> & drive_steer_brake,
		float sDamping,float sStiffness,
		float sCompression,
		float sRestLength,
		float rollInfluence,
		float wheel_width,
		float wheel_friction,
		float wheel_radius,
		std::vector<std::shared_ptr<Object>> & wheel,
		glm::vec3 upVector)
{
	data.sceneID = 0;

	data.maxEngineForce = drive_steer_brake[0];
	data.engineForce = 0.0f;
	data.engineIncrement = drive_steer_brake[1];
	data.maxSteeringForce = drive_steer_brake[2];
	data.steering = 0.0f;
	data.steeringIncrement = drive_steer_brake[3];
	data.maxBrakeForce = drive_steer_brake[4];
	data.brakeForce = 0.0f;
	data.brakeIncrement = drive_steer_brake[5];

	data.suspension_damping = sDamping;
	data.suspension_stiffness = sStiffness;
	data.suspension_compression = sCompression;
	data.suspensionRestLength = sRestLength;
	data.roll_influence = rollInfluence;

	data.wheel_width = wheel_width;
	data.wheel_friction = wheel_friction;
	data.wheel_radius = wheel_radius;
	data.wheel = wheel;

	baseUp = upVector;
	up = baseUp;
}

void Vehicle::setScene(int id)
{
	data.sceneID = id;
}

void Vehicle::setName(std::string name)
{
	data.name = name;
}

void Vehicle::setWheels(btVector3 direction, btVector3 axis, std::vector<btVector3> & connectionPoint, std::array<bool, 4> & frontWheel)
{
	data.front = frontWheel;
	for(int i{0}; i < 4; ++i)
	{
		vehicle->addWheel(connectionPoint[i], direction, axis, data.suspensionRestLength, data.wheel_radius, tuning, frontWheel[i]);
	}

	for(int i{0}; i < 4; ++i)
	{
		btWheelInfo & wheelInfo = vehicle->getWheelInfo(i);
		wheelInfo.m_suspensionStiffness = data.suspension_stiffness;
		wheelInfo.m_wheelsDampingRelaxation = data.suspension_damping;
		wheelInfo.m_wheelsDampingCompression = data.suspension_compression;
		wheelInfo.m_frictionSlip = data.wheel_friction;
		wheelInfo.m_rollInfluence = data.roll_influence;
		wheelInfo.m_maxSuspensionForce = 3000.0f;
	}
}
