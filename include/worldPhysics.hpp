#ifndef GAME_PHYSICS_HPP
#define GAME_PHYSICS_HPP

#include <iostream>
#include <memory>
#include <utility>
#include "animatedObject.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

enum class COLLISION_SHAPE
{
	BOX,
	SPHERE,
	CAPSULE,
	CONVEX_HULL,
	COMPOUND,
	TRIANGLE,
	GIMPACT
};

enum class CHARACTER_DIRECTION
{
	FRONT,
	RIGHT,
	LEFT
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
		void addKinematicCharacter(std::shared_ptr<Object> object);
		void addRigidBody(std::shared_ptr<Object> object, glm::vec3 origin, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape);
		void stepSimulation();
		void stepSimulation(glm::mat4 & view, glm::mat4 & projection);
		glm::vec3 getObjectPosition(int objectIndex);
		glm::mat4 getObjectRotation(int objectIndex);
		glm::mat4 getObjectOpenGLMatrix(int objectIndex);
		int getNumRigidBody();
		glm::mat4 mainCharacterDoActionWalk(CHARACTER_DIRECTION direction);
		glm::mat4 mainCharacterDoActionRun(CHARACTER_DIRECTION direction);
		glm::mat4 mainCharacterDoActionIdle();

	private:
		btCollisionShape * createConvexHullShape(std::shared_ptr<Object> & object);
		btCollisionShape * createCompoundShape(std::shared_ptr<Object> & object);
		btCollisionShape * createTriangleShape(std::shared_ptr<Object> & object);
		btCollisionShape * createGImpactShape(std::shared_ptr<Object> & object);

		btDebugDraw debugDrawer;
		btSoftBodyRigidBodyCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
		btDbvtBroadphase * overlappingPairCache;
		btSequentialImpulseConstraintSolver * solver;
		btSoftRigidDynamicsWorld * dynamicsWorld;
		btSoftBodyWorldInfo * softBodyWorldInfo;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		btKinematicCharacterController * mainCharacter;
};

#endif
