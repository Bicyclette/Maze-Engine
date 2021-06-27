#ifndef GAME_PHYSICS_HPP
#define GAME_PHYSICS_HPP

#include <iostream>
#include <memory>
#include <utility>
#include "character.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

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
		void setKinematicCharacter(std::unique_ptr<AnimatedObject> & object);
		void removeKinematicCharacter();
		void addRigidBody(std::shared_ptr<Object> object, glm::mat4 position, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape);
		void addSoftBody(std::shared_ptr<Object> object, btScalar mass);
		void updateSoftBody(int softBodyIndex, std::shared_ptr<Object> object);
		void setSoftBodyVertexMass(int sbIndex, int vIndex, btScalar mass);
		void stepSimulation();
		void stepSimulation(glm::mat4 & view, glm::mat4 & projection);
		glm::mat4 getObjectOpenGLMatrix(int objectIndex);
		int getNumRigidBody();
		int getNumSoftBody();
		void characterDoActionWalk(std::shared_ptr<Character> hero, Character::DIRECTION d, float delta);
		void characterDoActionRun(std::shared_ptr<Character> hero, Character::DIRECTION d, float delta);
		void characterDoActionJump(std::shared_ptr<Character> hero, bool forward, float delta);
		void characterDoActionIdle(std::shared_ptr<Character> hero);

	private:
		btCollisionShape * createConvexHullShape(std::shared_ptr<Object> & object);
		btCollisionShape * createCompoundShape(std::shared_ptr<Object> & object);
		btCollisionShape * createTriangleShape(std::shared_ptr<Object> & object);

		btDebugDraw debugDrawer;
		btSoftBodyRigidBodyCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
		btDbvtBroadphase * overlappingPairCache;
		btSequentialImpulseConstraintSolver * solver;
		btSoftRigidDynamicsWorld * dynamicsWorld;
		btSoftBodyWorldInfo * softBodyWorldInfo;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		btKinematicCharacterController * character;
};

#endif
