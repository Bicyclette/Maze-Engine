#include "worldPhysics.hpp"

WorldPhysics::WorldPhysics() :
	collisionConfiguration(new btSoftBodyRigidBodyCollisionConfiguration()),
	dispatcher(new btCollisionDispatcher(collisionConfiguration)),
	overlappingPairCache(new btDbvtBroadphase()),
	solver(new btSequentialImpulseConstraintSolver()),
	dynamicsWorld(new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration)),
	softBodyWorldInfo(new btSoftBodyWorldInfo()),
	mainCharacter(nullptr)
{
	dynamicsWorld->setGravity(btVector3(0.0, -10.0, 0.0));
	softBodyWorldInfo->m_broadphase = overlappingPairCache;
	softBodyWorldInfo->m_dispatcher = dispatcher;
	softBodyWorldInfo->m_sparsesdf.Initialize();

	debugDrawer.setDebugMode(1);
	dynamicsWorld->setDebugDrawer(&debugDrawer);
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

btCollisionShape * WorldPhysics::createConvexHullShape(std::shared_ptr<Object> & object)
{
	btConvexHullShape * shape = new btConvexHullShape();

	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	for(int i{0}; i < meshes.size(); ++i)
	{
		const std::vector<Vertex> & vertices = meshes[i]->getVertices();
		const std::vector<int> & indices = meshes[i]->getIndices();

		for(int j{0}; j < indices.size(); j+=3)
		{
			btVector3 v0(vertices[indices[j]].position.x, vertices[indices[j]].position.y, vertices[indices[j]].position.z);
			btVector3 v1(vertices[indices[j+1]].position.x, vertices[indices[j+1]].position.y, vertices[indices[j+1]].position.z);
			btVector3 v2(vertices[indices[j+2]].position.x, vertices[indices[j+2]].position.y, vertices[indices[j+2]].position.z);

			shape->addPoint(v0);
			shape->addPoint(v1);
			shape->addPoint(v2);
		}
	}

	return shape;
}

btCollisionShape * WorldPhysics::createCompoundShape(std::shared_ptr<Object> & object)
{
	btCompoundShape * shape = new btCompoundShape();
	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	for(int i{0}; i < meshes.size(); ++i)
	{
		const std::vector<Vertex> & vertices = meshes[i]->getVertices();
		const std::vector<int> & indices = meshes[i]->getIndices();

		btConvexHullShape * childShape = new btConvexHullShape();
		for(int j{0}; j < indices.size(); j+=3)
		{
			btVector3 v0(vertices[indices[j]].position.x, vertices[indices[j]].position.y, vertices[indices[j]].position.z);
			btVector3 v1(vertices[indices[j+1]].position.x, vertices[indices[j+1]].position.y, vertices[indices[j+1]].position.z);
			btVector3 v2(vertices[indices[j+2]].position.x, vertices[indices[j+2]].position.y, vertices[indices[j+2]].position.z);

			childShape->addPoint(v0);
			childShape->addPoint(v1);
			childShape->addPoint(v2);
		}

		btTransform childTransform;
		childTransform.setIdentity();
		childTransform.setOrigin(btVector3(0, 0, 0));

		shape->addChildShape(childTransform, childShape);
	}

	return shape;
}

btCollisionShape * WorldPhysics::createTriangleShape(std::shared_ptr<Object> & object)
{
	btTriangleMesh * triangleMesh = new btTriangleMesh();

	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	for(int i{0}; i < meshes.size(); ++i)
	{
		const std::vector<Vertex> & vertices = meshes[i]->getVertices();
		const std::vector<int> & indices = meshes[i]->getIndices();

		for(int j{0}; j < indices.size(); j+=3)
		{
			btVector3 v0(vertices[indices[j]].position.x, vertices[indices[j]].position.y, vertices[indices[j]].position.z);
			btVector3 v1(vertices[indices[j+1]].position.x, vertices[indices[j+1]].position.y, vertices[indices[j+1]].position.z);
			btVector3 v2(vertices[indices[j+2]].position.x, vertices[indices[j+2]].position.y, vertices[indices[j+2]].position.z);

			triangleMesh->addTriangle(v0, v1, v2);
			triangleMesh->addTriangleIndices(j, j+1, j+2);
		}
	}

	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(triangleMesh, true);
	return shape;
}

btCollisionShape * WorldPhysics::createGImpactShape(std::shared_ptr<Object> & object)
{
	btTriangleMesh * triangleMesh = new btTriangleMesh();

	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	for(int i{0}; i < meshes.size(); ++i)
	{
		const std::vector<Vertex> & vertices = meshes[i]->getVertices();
		const std::vector<int> & indices = meshes[i]->getIndices();

		for(int j{0}; j < indices.size(); j+=3)
		{
			btVector3 v0(vertices[indices[j]].position.x, vertices[indices[j]].position.y, vertices[indices[j]].position.z);
			btVector3 v1(vertices[indices[j+1]].position.x, vertices[indices[j+1]].position.y, vertices[indices[j+1]].position.z);
			btVector3 v2(vertices[indices[j+2]].position.x, vertices[indices[j+2]].position.y, vertices[indices[j+2]].position.z);

			triangleMesh->addTriangle(v0, v1, v2);
			triangleMesh->addTriangleIndices(j, j+1, j+2);
		}
	}

	btGImpactMeshShape * shape = new btGImpactMeshShape(triangleMesh);
	shape->updateBound();
	return shape;
}

void WorldPhysics::addRigidBody(std::shared_ptr<Object> object, glm::vec3 origin, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape)
{
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;
	
	btCollisionShape * shape = nullptr;
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	if(collision_shape == COLLISION_SHAPE::BOX)
		shape = new btBoxShape(btVector3(xExtent/2.0f, yExtent/2.0f, zExtent/2.0f));
	else if(collision_shape == COLLISION_SHAPE::SPHERE)
		shape = new btSphereShape(btScalar(xExtent/2.0f));
	else if(collision_shape == COLLISION_SHAPE::CAPSULE)
		shape = new btCapsuleShape(btScalar(xExtent/2.0f), btScalar(yExtent - (xExtent/2.0f)));
	else if(collision_shape == COLLISION_SHAPE::CONVEX_HULL)
		shape = createConvexHullShape(object);
	else if(collision_shape == COLLISION_SHAPE::COMPOUND)
		shape = createCompoundShape(object);
	else if(collision_shape == COLLISION_SHAPE::TRIANGLE)
		shape = createTriangleShape(object);
	else if(collision_shape == COLLISION_SHAPE::GIMPACT)
		shape = createGImpactShape(object);
	else
	{
		std::cerr << "Failed to add a rigid body to scene !" << std::endl;
		return;
	}
	collisionShapes.push_back(shape);

	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if(mass != 0.0f)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	btDefaultMotionState * motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	rbInfo.m_restitution = restitution;
	btRigidBody * body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);
}

void WorldPhysics::addKinematicCharacter(std::shared_ptr<Object> object)
{
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;

	// main character
	btPairCachingGhostObject * ghostObject = new btPairCachingGhostObject();
	btCapsuleShape * convexShape = new btCapsuleShape(btScalar(xExtent/2.0f), btScalar(yExtent - (xExtent/2.0f)));
	mainCharacter = new btKinematicCharacterController(ghostObject, convexShape, btScalar(0.25));
	mainCharacter->setGravity(btVector3(0, -10, 0));
	mainCharacter->debugDraw(&debugDrawer);
}

void WorldPhysics::stepSimulation()
{
	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
}

void WorldPhysics::stepSimulation(glm::mat4 & view, glm::mat4 & projection)
{
	debugDrawer.setViewMatrix(view);
	debugDrawer.setProjectionMatrix(projection);

	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
	dynamicsWorld->debugDrawWorld();
}

glm::vec3 WorldPhysics::getObjectPosition(int objectIndex)
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

glm::mat4 WorldPhysics::getObjectRotation(int objectIndex)
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

glm::mat4 WorldPhysics::getObjectOpenGLMatrix(int objectIndex)
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

	glm::mat4 matrix;
	transform.getOpenGLMatrix(glm::value_ptr(matrix));
	return matrix;
}

int WorldPhysics::getNumRigidBody()
{
	return collisionShapes.size();
}

// ######################################################################
// ######################################################################
// ######################################################################

btDebugDraw::btDebugDraw() :
	shader("../shaders/axis/vertex.glsl", "../shaders/axis/fragment.glsl")
{}

void btDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	float line[6] =
	{
		from.getX(),
		from.getY(),
		from.getZ(),
		to.getX(),
		to.getY(),
		to.getZ()
	};

	GLuint vao;
	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	// draw
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	shader.use();
	shader.setVec3f("color", glm::vec3(color.getX(), color.getY(), color.getZ()));
	shader.setMatrix("model", glm::mat4(1.0f));
	shader.setMatrix("view", view);
	shader.setMatrix("proj", projection);

	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// cleaning
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void btDebugDraw::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
	// contact point's normal
	float normal[6] =
	{
		PointOnB.getX(),
		PointOnB.getY(),
		PointOnB.getZ(),
		PointOnB.getX() + normalOnB.getX(),
		PointOnB.getY() + normalOnB.getY(),
		PointOnB.getZ() + normalOnB.getZ()
	};

	GLuint vao;
	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), normal, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	// draw
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	shader.use();
	shader.setVec3f("color", glm::vec3(color.getX(), color.getY(), color.getZ()));
	shader.setMatrix("model", glm::mat4(1.0f));
	shader.setMatrix("view", view);
	shader.setMatrix("proj", projection);

	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// cleaning
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	// Contact point
	float point[3] =
	{
		PointOnB.getX(),
		PointOnB.getY(),
		PointOnB.getZ()
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), point, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	// draw
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPointSize(5.0f);

	shader.use();
	shader.setVec3f("color", glm::vec3(color.getX(), color.getY(), color.getZ()));
	shader.setMatrix("model", glm::mat4(1.0f));
	shader.setMatrix("view", view);
	shader.setMatrix("proj", projection);

	glDrawArrays(GL_POINTS, 0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// cleaning
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void btDebugDraw::reportErrorWarning(const char * warningString)
{
	std::cerr << ">>>>> BULLET REPORT ERROR WARNING : " << warningString << std::endl;
}

void btDebugDraw::draw3dText(const btVector3 & location, const char * textString)
{
	// TO DO...
}

void btDebugDraw::setDebugMode(int debugMode)
{
	mode = debugMode;
}

int btDebugDraw::getDebugMode() const
{
	return mode;
}

void btDebugDraw::setViewMatrix(glm::mat4 & m)
{
	view = m;
}

void btDebugDraw::setProjectionMatrix(glm::mat4 & m)
{
	projection = m;
}