#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

class Object;

enum class VEHICLE_STEERING
{
	NONE,
	RIGHT,
	LEFT
};

struct VehicleData
{
	int sceneID;
	std::string name;

	// drive & steer
	float maxEngineForce;
	float engineForce;
	float engineIncrement;
	float maxSteeringForce;
	float steering;
	float steeringIncrement;
	float maxBrakeForce;
	float brakeForce;
	float brakeIncrement;

	// suspension data
	float suspension_damping;
	float suspension_stiffness;
	float suspension_compression;
	float suspensionRestLength;
	float roll_influence;

	// wheel data
	float wheel_width;
	float wheel_friction;
	float wheel_radius;
	std::array<bool, 4> front;
	std::vector<std::shared_ptr<Object>> wheel;
};

class Vehicle
{
	public:
		Vehicle(
				std::array<float, 6> & drive_steer_brake,
				float sDamping,
				float sStiffness,
				float sCompression,
				float sRestLength,
				float rollInfluence,
				float wheel_width,
				float wheel_friction,
				float wheel_radius,
				std::vector<std::shared_ptr<Object>> & wheel,
				glm::vec3 upVector);
		void setScene(int id);
		void setName(std::string name);
		void setWheels(btVector3 direction, btVector3 axis, std::vector<btVector3> & connectionPoint, std::array<bool, 4> & frontWheel);
	
	public:
		struct VehicleData data;
		btRaycastVehicle * vehicle;
		btRaycastVehicle::btVehicleTuning tuning;
		btVehicleRaycaster * raycaster;
		glm::vec3 baseUp;
		glm::vec3 up;
};
