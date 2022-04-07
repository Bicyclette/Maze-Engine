#ifndef GAME_PHYSICS_HPP
#define GAME_PHYSICS_HPP

#include <array>
#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>
#include "character.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include "vehicle.hpp"

enum class COLLISION_SHAPE
{
	BOX,
	SPHERE,
	CAPSULE,
	CYLINDER,
	CONVEX_HULL,
	COMPOUND,
	TRIANGLE
};

class btDebugDraw : public btIDebugDraw
{
	public:
		btDebugDraw();
		void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color);
		void drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color);
		void reportErrorWarning(const char * warningString);
		void draw3dText(const btVector3 & location, const char * textString);
		void setDebugMode(int debugMode);
		int getDebugMode() const;
		void setViewMatrix(glm::mat4 & m);
		void setProjectionMatrix(glm::mat4 & m);

	private:
		int mode;
		Shader shader;
		glm::mat4 view;
		glm::mat4 projection;
};

class WorldPhysics
{
	public:
		WorldPhysics();
		~WorldPhysics();
		void setKinematicCharacter(std::unique_ptr<AnimatedObject> & object, glm::mat4 model = glm::mat4(1.0f));
		void removeKinematicCharacter();
		void addRigidBody(std::shared_ptr<Object> object, glm::mat4 position, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape);
		void addSoftBody(std::shared_ptr<Object> object, btScalar mass);
		void updateSoftBody(int softBodyIndex, std::shared_ptr<Object> object);
		void setSoftBodyVertexMass(int sbIndex, int vIndex, btScalar mass);
		void attachVertexSoftBody(int sbIndex, int rbIndex, int vIndex, bool disableCollision = false);
		void stepSimulation();
		void stepSimulation(glm::mat4 & view, glm::mat4 & projection);
		glm::mat4 getObjectOpenGLMatrix(int objectIndex);
		int getNumRigidBody();
		int getNumSoftBody();
		void characterDoActionWalk(std::shared_ptr<Character> hero, Character::DIRECTION d, float delta);
		void characterDoActionRun(std::shared_ptr<Character> hero, Character::DIRECTION d, float delta);
		void characterDoActionJump(std::shared_ptr<Character> hero, bool forward, float delta);
		void characterDoActionIdle(std::shared_ptr<Character> hero);
		std::shared_ptr<Vehicle> addVehicle(
				std::array<float, 6> & drive_steer_brake,
				float sDamping,
				float sStiffness,
				float sCompression,
				float sRestLength,
				float rollInfluence,
				float wheel_width,
				float wheel_friction,
				float wheel_radius,
				btVector3 wheelAxis,
				std::vector<btVector3> & connectionPoint,
				std::array<bool, 4> & frontWheel,
				int rbChassisIndex,
				std::vector<std::shared_ptr<Object>> wheel = std::vector<std::shared_ptr<Object>>(),
				glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f));
		void vehicleDrive(std::shared_ptr<Vehicle> & v, bool forward);
		void vehicleDriveReset(std::shared_ptr<Vehicle> & v);
		void vehicleSteering(std::shared_ptr<Vehicle> & v, VEHICLE_STEERING dir);
		void vehicleSteeringReset(std::shared_ptr<Vehicle> & v);
		void setVehicleWheelTransform(std::shared_ptr<Vehicle> & v);
		void updateVehicleUpVector(std::shared_ptr<Vehicle> & v);

        void addSliderConstraint(int rbIndexA, int rbIndexB, float lowerLinLimit, float upperLinLimit, float lowerAngLimit, float upperAngLimit);
        void applyImpulse(int rbIndex, btVector3 impulse, btVector3 relPos);
        float getDistance(btVector3 from, btVector3 to);

	private:
		btCollisionShape * createConvexHullShape(std::shared_ptr<Object>& object);
		btCollisionShape * createCompoundShape(std::shared_ptr<Object>& object);
		btCollisionShape * createTriangleShape(std::shared_ptr<Object>& object);
		void stepSimulationAux();

		btDebugDraw debugDrawer;
		btSoftBodyRigidBodyCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
		btDbvtBroadphase * overlappingPairCache;
		btSequentialImpulseConstraintSolver * solver;
		btSoftRigidDynamicsWorld * dynamicsWorld;
		btSoftBodyWorldInfo * softBodyWorldInfo;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		btAlignedObjectArray<btRigidBody*> rigidBodies;
		btKinematicCharacterController * character;

		std::vector<std::shared_ptr<Object>> worldRigidBody;
		std::vector<std::shared_ptr<Object>> worldSoftBody;
};

#endif
