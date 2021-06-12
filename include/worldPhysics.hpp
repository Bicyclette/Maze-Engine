#ifndef GAME_PHYSICS_HPP
#define GAME_PHYSICS_HPP

#include <iostream>
#include <memory>
#include <utility>
#include "animatedObject.hpp"
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

enum class COLLISION_SHAPE
{
	PLANE,
	BOX,
	SPHERE,
	CAPSULE
};

class WorldPhysics
{
	public:
		WorldPhysics();
		~WorldPhysics();
		void addRigidBody(std::shared_ptr<Object> object, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape);
		void stepSimulation();
		glm::vec3 getObjectPosition(int sceneIndex, int objectIndex);
		glm::mat4 getObjectRotation(int sceneIndex, int objectIndex);

	private:
		btSoftBodyRigidBodyCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
		btDbvtBroadphase * overlappingPairCache;
		btSequentialImpulseConstraintSolver * solver;
		btSoftRigidDynamicsWorld * dynamicsWorld;
		btSoftBodyWorldInfo * softBodyWorldInfo;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif
