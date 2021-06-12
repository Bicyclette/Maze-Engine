#include "worldPhysics.hpp"

WorldPhysics::WorldPhysics() :
	collisionConfiguration(new btSoftBodyRigidBodyCollisionConfiguration()),
	dispatcher(new btCollisionDispatcher(collisionConfiguration)),
	overlappingPairCache(new btDbvtBroadphase()),
	solver(new btSequentialImpulseConstraintSolver()),
	dynamicsWorld(new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration)),
	softBodyWorldInfo(new btSoftBodyWorldInfo())
{
	dynamicsWorld->setGravity(btVector3(0.0, -10.0, 0.0));
	softBodyWorldInfo->m_broadphase = overlappingPairCache;
	softBodyWorldInfo->m_dispatcher = dispatcher;
	softBodyWorldInfo->m_sparsesdf.Initialize();
}

WorldPhysics::~WorldPhysics()
{
	// cleanup in the reverse order of creation/initialization
	// remove rigidbodies from the dynamics world and delete them
    for(int i{0}; i < dynamicsWorld->getNumCollisionObjects(); ++i)
    {
        btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody * body = btRigidBody::upcast(obj);
        if(body && body->getMotionState())
        {
            delete(body->getMotionState());
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete(obj);
    }

    // delete collision shapes
    for(int i{0}; i < collisionShapes.size(); ++i)
    {
        btCollisionShape * shape = collisionShapes[i];
        collisionShapes[i] = nullptr;
        delete(shape);
    }
	collisionShapes.clear();

    delete(dynamicsWorld);
    delete(solver);
    delete(overlappingPairCache);
    delete(dispatcher);
    delete(collisionConfiguration);	
	delete(softBodyWorldInfo);
}

void WorldPhysics::addRigidBody(std::shared_ptr<Object> object, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape)
{
	btCollisionShape * shape = nullptr;
	glm::vec3 origin = object->getOrigin();
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;
	if(collision_shape == COLLISION_SHAPE::BOX)
	{
		shape = new btBoxShape(btVector3(xExtent/2.0f, yExtent/2.0f, zExtent/2.0f));
	}
	else if(collision_shape == COLLISION_SHAPE::SPHERE)
	{
		shape = new btSphereShape(btScalar(xExtent/2.0f));
	}
	else
	{
		std::cerr << "Failed to add a rigid body to scene !" << std::endl;
		return;
	}
	collisionShapes.push_back(shape);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if(mass != 0.0f)
	{
		shape->calculateLocalInertia(mass, localInertia);
		transform.setOrigin(btVector3(origin.x, origin.y, origin.z));
	}

	btDefaultMotionState * motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	rbInfo.m_restitution = restitution;
	btRigidBody * body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);
}

void WorldPhysics::stepSimulation()
{
	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
}

glm::vec3 WorldPhysics::getObjectPosition(int sceneIndex, int objectIndex)
{
	btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[objectIndex];
	btRigidBody * body = btRigidBody::upcast(obj);
	btTransform transform;
	if(body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(transform);
	}
	else
	{
		transform = obj->getWorldTransform();
	}

	btVector3 origin = transform.getOrigin();
	return glm::vec3(origin.getX(), origin.getY(), origin.getZ());
}

glm::mat4 WorldPhysics::getObjectRotation(int sceneIndex, int objectIndex)
{
	btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[objectIndex];
	btRigidBody * body = btRigidBody::upcast(obj);
	btTransform transform;
	if(body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(transform);
	}
	else
	{
		transform = obj->getWorldTransform();
	}

	btQuaternion rotation = transform.getRotation();
	float angle = rotation.getAngle();
	btVector3 axis = rotation.getAxis();
	return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(axis.getX(), axis.getY(), axis.getZ()));
}
