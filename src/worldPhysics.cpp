#include "worldPhysics.hpp"

WorldPhysics::WorldPhysics() :
	collisionConfiguration(new btSoftBodyRigidBodyCollisionConfiguration()),
	dispatcher(new btCollisionDispatcher(collisionConfiguration)),
	overlappingPairCache(new btDbvtBroadphase()),
	solver(new btSequentialImpulseConstraintSolver()),
	dynamicsWorld(new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration)),
	softBodyWorldInfo(new btSoftBodyWorldInfo()),
	character(nullptr)
{
	dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
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

btCollisionShape * WorldPhysics::createConvexHullShape(std::shared_ptr<Object>& object)
{
	btConvexHullShape * shape = new btConvexHullShape();

	std::vector<std::shared_ptr<Mesh>> meshes = (object->getCollisionShape()) ?
		object->getCollisionShape()->getMeshes() : object->getMeshes();
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

btCollisionShape * WorldPhysics::createCompoundShape(std::shared_ptr<Object>& object)
{
	btCompoundShape * shape = new btCompoundShape();
	std::vector<std::shared_ptr<Mesh>> meshes = (object->getCollisionShape()) ?
		object->getCollisionShape()->getMeshes() : object->getMeshes();
	std::vector<glm::mat4> instances = object->getInstanceModel();
	if(instances.size() == 0)
	{
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
	}
	else
	{
		for(int i{0}; i < instances.size(); ++i)
		{
			glm::mat4 model = instances[i];
			for(int j{0}; j < meshes.size(); ++j)
			{
				const std::vector<Vertex> & vertices = meshes[j]->getVertices();
				const std::vector<int> & indices = meshes[j]->getIndices();

				btConvexHullShape * childShape = new btConvexHullShape();
				for(int k{0}; k < indices.size(); k+=3)
				{
					glm::vec4 vertex0 = model * glm::vec4(vertices[indices[k]].position.x, vertices[indices[k]].position.y, vertices[indices[k]].position.z, 1.0f);
					glm::vec4 vertex1 = model * glm::vec4(vertices[indices[k+1]].position.x, vertices[indices[k+1]].position.y, vertices[indices[k+1]].position.z, 1.0f);
					glm::vec4 vertex2 = model * glm::vec4(vertices[indices[k+2]].position.x, vertices[indices[k+2]].position.y, vertices[indices[k+2]].position.z, 1.0f);
					
					btVector3 v0(vertex0.x, vertex0.y, vertex0.z);
					btVector3 v1(vertex1.x, vertex1.y, vertex1.z);
					btVector3 v2(vertex2.x, vertex2.y, vertex2.z);

					childShape->addPoint(v0);
					childShape->addPoint(v1);
					childShape->addPoint(v2);
				}

				btTransform childTransform;
				childTransform.setIdentity();
				childTransform.setOrigin(btVector3(0, 0, 0));

				shape->addChildShape(childTransform, childShape);
			}
		}
	}

	return shape;
}

btCollisionShape * WorldPhysics::createTriangleShape(std::shared_ptr<Object>& object)
{
	btTriangleMesh * triangleMesh = new btTriangleMesh();

	std::vector<std::shared_ptr<Mesh>> meshes = (object->getCollisionShape()) ?
		object->getCollisionShape()->getMeshes() : object->getMeshes();

	std::vector<glm::mat4> instances = object->getInstanceModel();
	if(instances.size() == 0)
	{
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
	}
	else
	{
		for(int i{0}; i < instances.size(); ++i)
		{
			glm::mat4 model = instances[i];
			for(int j{0}; j < meshes.size(); ++j)
			{
				const std::vector<Vertex> & vertices = meshes[j]->getVertices();
				const std::vector<int> & indices = meshes[j]->getIndices();

				for(int k{0}; k < indices.size(); k+=3)
				{
					glm::vec4 vertex0 = model * glm::vec4(vertices[indices[k]].position.x, vertices[indices[k]].position.y, vertices[indices[k]].position.z, 1.0f);
					glm::vec4 vertex1 = model * glm::vec4(vertices[indices[k+1]].position.x, vertices[indices[k+1]].position.y, vertices[indices[k+1]].position.z, 1.0f);
					glm::vec4 vertex2 = model * glm::vec4(vertices[indices[k+2]].position.x, vertices[indices[k+2]].position.y, vertices[indices[k+2]].position.z, 1.0f);
					
					btVector3 v0(vertex0.x, vertex0.y, vertex0.z);
					btVector3 v1(vertex1.x, vertex1.y, vertex1.z);
					btVector3 v2(vertex2.x, vertex2.y, vertex2.z);

					triangleMesh->addTriangle(v0, v1, v2);
					triangleMesh->addTriangleIndices(k, k+1, k+2);
				}
			}
		}
	}

	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(triangleMesh, true);
	return shape;
}

void WorldPhysics::addRigidBody(std::shared_ptr<Object> object, glm::mat4 position, btScalar mass, btScalar restitution, COLLISION_SHAPE collision_shape)
{
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;
	
	btCollisionShape * shape = nullptr;
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(position));

	if(collision_shape == COLLISION_SHAPE::BOX)
		shape = new btBoxShape(btVector3(xExtent/2.0f, yExtent/2.0f, zExtent/2.0f));
	else if(collision_shape == COLLISION_SHAPE::SPHERE)
		shape = new btSphereShape(btScalar(xExtent/2.0f));
	else if(collision_shape == COLLISION_SHAPE::CAPSULE)
		shape = new btCapsuleShape(btScalar(xExtent/2.0f), btScalar(yExtent - (xExtent/2.0f)));
	else if(collision_shape == COLLISION_SHAPE::CYLINDER)
		shape = new btCylinderShape(btVector3(xExtent/2.0f, yExtent/2.0f, zExtent/2.0f));
	else if(collision_shape == COLLISION_SHAPE::CONVEX_HULL)
		shape = createConvexHullShape(object);
	else if(collision_shape == COLLISION_SHAPE::COMPOUND)
		shape = createCompoundShape(object);
	else if(collision_shape == COLLISION_SHAPE::TRIANGLE)
		shape = createTriangleShape(object);
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
	rigidBodies.push_back(body);
	worldRigidBody.push_back(object);
}

void WorldPhysics::addSoftBody(std::shared_ptr<Object> object, btScalar mass)
{
	std::function getVertexIndex = [] (std::vector<Vertex> & vertices, glm::vec3 & pos) -> int {
		int id{0};
		for(auto v : vertices)
		{
			if(v.position == pos)
				return id;
			id++;
		}
		return -1;
	};

	// remove duplicated vertices
	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	std::vector<Vertex> prevVertex = meshes[0]->getVertices();
	std::vector<int> prevIndex = meshes[0]->getIndices();
	std::vector<Vertex> cleanVertex;
	std::vector<int> cleanIndex;
	for(int i{0}; i < prevIndex.size(); i+=3)
	{
		int idx[3] = {prevIndex[i], prevIndex[i+1], prevIndex[i+2]};
		Vertex v[3] = {prevVertex[idx[0]], prevVertex[idx[1]], prevVertex[idx[2]]};

		for(int j{0}; j < 3; ++j)
		{
			int id = getVertexIndex(cleanVertex, v[j].position);
			if(id == -1)
			{
				cleanVertex.push_back(v[j]);
				cleanIndex.push_back(cleanVertex.size() - 1);
			}
			else
			{
				cleanIndex.push_back(id);
			}
		}
	}

	// create soft body
	std::vector<btScalar> vertexArray;
	for(auto v : cleanVertex)
	{
		vertexArray.push_back(v.position.x);
		vertexArray.push_back(v.position.y);
		vertexArray.push_back(v.position.z);
	}
	btSoftBody * softBody = btSoftBodyHelpers::CreateFromTriMesh(*softBodyWorldInfo, vertexArray.data(), cleanIndex.data(), cleanIndex.size() / 3);

	// define soft body material
	softBody->m_materials[0]->m_kLST = 0.75f;
	softBody->m_materials[0]->m_kAST = 0.0f;
	softBody->m_materials[0]->m_kVST = 0.75f;
	
	softBody->m_cfg.aeromodel = btSoftBody::eAeroModel::V_Point; // defines what kind of feature is used to compute aerodynamic forces
	//softBody->m_cfg.kMT = 0.0f; // pose matching coefficient [0,1]
	//softBody->m_cfg.kDP = 0.0f; // damping coefficient [0,1]
	//softBody->m_cfg.kDF = 0.5f; // dynamic friction coefficient [0,1]
	//softBody->m_cfg.kPR = 10.0f; // pressure coefficient [-inf,+inf]
	//softBody->m_cfg.kVC = 5.0f; // volume conservation coefficient [0,+inf]
	softBody->m_cfg.kKHR = 0.5f; // kinetic contacts hardness [0,1]
	
	softBody->generateBendingConstraints(2);
	softBody->generateClusters(0);
	softBody->setPose(true, false);
	softBody->setTotalMass(mass, false);

	// add soft body to dynamics world
	dynamicsWorld->addSoftBody(softBody);
	worldSoftBody.push_back(object);
	
	// recreate mesh
	btSoftBody::tNodeArray nodes = softBody->m_nodes;
	btSoftBody::tFaceArray faces = softBody->m_faces;
	std::vector<Vertex> updatedVertices;
	std::vector<int> updatedIndices;

	for(int i{0}; i < nodes.size(); ++i)
	{
		glm::vec3 pos(nodes[i].m_x.x(), nodes[i].m_x.y(), nodes[i].m_x.z());
		glm::vec3 normal(nodes[i].m_n.x(), nodes[i].m_n.y(), nodes[i].m_n.z());
		glm::vec3 lastPos(nodes[i].m_q.x(), nodes[i].m_q.y(), nodes[i].m_q.z());

		Vertex v;
		if(meshes[0]->getVertex(pos, normal, lastPos, v))
			updatedVertices.push_back(v);
	}
	for(int i{0}; i < faces.size(); ++i)
	{
		for(int n{0}; n < 3; ++n)
		{
			btSoftBody::Node * node = faces[i].m_n[n];
			glm::vec3 pos(node->m_x.x(), node->m_x.y(), node->m_x.z());
			for(int id{0}; id < updatedVertices.size(); ++id)
			{
				glm::vec3 vertexPos = updatedVertices[id].position;
				if(pos == vertexPos)
				{
					updatedIndices.push_back(id);
					break;
				}
			}
		}
	}

	meshes[0]->recreate(updatedVertices, updatedIndices, true);
}

void WorldPhysics::updateSoftBody(int softBodyIndex, std::shared_ptr<Object> object)
{
	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	std::shared_ptr<Mesh> mesh = meshes[0];

	btSoftBodyArray softBodyArray = dynamicsWorld->getSoftBodyArray();
	btSoftBody * softBody = softBodyArray[softBodyIndex];
	btSoftBody::tNodeArray nodes = softBody->m_nodes;
	btSoftBody::tFaceArray faces = softBody->m_faces;
	std::vector<Vertex> updatedVertices;
	std::vector<int> updatedIndices;

	for(int i{0}; i < nodes.size(); ++i)
	{
		glm::vec3 pos(nodes[i].m_x.x(), nodes[i].m_x.y(), nodes[i].m_x.z());
		glm::vec3 normal(nodes[i].m_n.x(), nodes[i].m_n.y(), nodes[i].m_n.z());
		glm::vec3 lastPos(nodes[i].m_q.x(), nodes[i].m_q.y(), nodes[i].m_q.z());

		Vertex v;
		if(mesh->getVertex(pos, normal, lastPos, v))
			updatedVertices.push_back(v);
	}
	for(int i{0}; i < faces.size(); ++i)
	{
		for(int n{0}; n < 3; ++n)
		{
			btSoftBody::Node * node = faces[i].m_n[n];
			glm::vec3 pos(node->m_x.x(), node->m_x.y(), node->m_x.z());
			for(int id{0}; id < updatedVertices.size(); ++id)
			{
				glm::vec3 vertexPos = updatedVertices[id].position;
				if(pos == vertexPos)
				{
					updatedIndices.push_back(id);
					break;
				}
			}
		}
	}
	mesh->updateVBO(updatedVertices, updatedIndices);
}

void WorldPhysics::setSoftBodyVertexMass(int sbIndex, int vIndex, btScalar mass)
{
	btSoftBodyArray softBodyArray = dynamicsWorld->getSoftBodyArray();
	btSoftBody * softBody = softBodyArray[sbIndex];

	softBody->setMass(vIndex, mass);
/*
	btSoftBody::tNodeArray nodes = softBody->m_nodes;
	for(int i{0}; i < nodes.size(); ++i)
	{
		glm::vec3 pos(nodes[i].m_x.x(), nodes[i].m_x.y(), nodes[i].m_x.z());
		std::cout << "node " << i << " : " << glm::to_string(pos) << std::endl;
	}
*/
}

void WorldPhysics::attachVertexSoftBody(int sbIndex, int rbIndex, int vIndex, bool disableCollision)
{
	btSoftBodyArray softBodyArray = dynamicsWorld->getSoftBodyArray();
	if(softBodyArray.size() == 0)
	{
		std::cerr << "Error : there are no soft bodies in current world !" << std::endl;
		return;
	}
	btSoftBody * softBody = softBodyArray[sbIndex];

	btRigidBody * rigidBody = rigidBodies[rbIndex];

	softBody->appendAnchor(vIndex, rigidBody, disableCollision);
}

void WorldPhysics::setKinematicCharacter(std::unique_ptr<AnimatedObject> & object, glm::mat4 model)
{
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;

	// main character
	btCapsuleShape * convexShape = new btCapsuleShapeZ(1, 2);
	btPairCachingGhostObject * ghostObject = new btPairCachingGhostObject();
	btTransform ghostObjectTransform;
	ghostObjectTransform.setFromOpenGLMatrix(glm::value_ptr(model));
	ghostObject->setWorldTransform(ghostObjectTransform);
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	character = new btKinematicCharacterController(ghostObject, convexShape, btScalar(0.25));
	character->setUp(btVector3(0,1,0));
	dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(character);

	character->debugDraw(&debugDrawer);
}

void WorldPhysics::removeKinematicCharacter()
{
	dynamicsWorld->removeAction(character);
	delete(character);
}

void WorldPhysics::stepSimulation()
{
	// step
	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
	stepSimulationAux();
}

void WorldPhysics::stepSimulation(glm::mat4 & view, glm::mat4 & projection)
{
	debugDrawer.setViewMatrix(view);
	debugDrawer.setProjectionMatrix(projection);

	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
	dynamicsWorld->debugDrawWorld();

	stepSimulationAux();
}

void WorldPhysics::stepSimulationAux()
{
	// RIGID BODIES
	for(int i{0}; i < worldRigidBody.size(); ++i)
	{
		glm::mat4 model = getObjectOpenGLMatrix(i);
		worldRigidBody[i]->setModel(model);
	}
	// SOFT BODIES
	for(int i{0}; i < worldSoftBody.size(); ++i)
	{
		updateSoftBody(i, worldSoftBody[i]);
	}
}

glm::mat4 WorldPhysics::getObjectOpenGLMatrix(int objectIndex)
{
	btRigidBody * body = rigidBodies[objectIndex];
	btTransform transform;
	if(body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(transform);
	}
	else
	{
		transform = body->getWorldTransform();
	}

	glm::mat4 matrix;
	transform.getOpenGLMatrix(glm::value_ptr(matrix));
	return matrix;
}

int WorldPhysics::getNumRigidBody()
{
	return collisionShapes.size();
}

int WorldPhysics::getNumSoftBody()
{
	return dynamicsWorld->getSoftBodyArray().size();
}

void WorldPhysics::characterDoActionWalk(std::shared_ptr<Character> hero, Character::DIRECTION direction, float delta)
{
	glm::vec3 & d{hero->walkDirection};

	if(direction == Character::DIRECTION::RIGHT)
	{
		hero->turnAngle += hero->turnSpeed * hero->walkSpeed;
		hero->walkDirection = glm::rotate(hero->walkDirection, hero->turnSpeed * hero->walkSpeed, glm::vec3(0, 1, 0));
	}
	else if(direction == Character::DIRECTION::LEFT)
	{
		hero->turnAngle -= hero->turnSpeed * hero->walkSpeed;
		hero->walkDirection = glm::rotate(hero->walkDirection, - (hero->turnSpeed * hero->walkSpeed), glm::vec3(0, 1, 0));
	}

	character->setWalkDirection(btVector3(d.x, d.y, d.z) * hero->walkSpeed * delta);
	btTransform transform = character->getGhostObject()->getWorldTransform();
	
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, hero->turnAngle, glm::vec3(0, 1, 0));
	hero->setModel(model);
}

void WorldPhysics::characterDoActionRun(std::shared_ptr<Character> hero, Character::DIRECTION direction, float delta)
{
	glm::vec3 & d{hero->walkDirection};

	if(direction == Character::DIRECTION::RIGHT)
	{
		hero->turnAngle += hero->turnSpeed * hero->runSpeed;
		hero->walkDirection = glm::rotate(hero->walkDirection, hero->turnSpeed * hero->runSpeed, glm::vec3(0, 1, 0));
	}
	else if(direction == Character::DIRECTION::LEFT)
	{
		hero->turnAngle -= hero->turnSpeed * hero->runSpeed;
		hero->walkDirection = glm::rotate(hero->walkDirection, - (hero->turnSpeed * hero->runSpeed), glm::vec3(0, 1, 0));
	}

	character->setWalkDirection(btVector3(d.x, d.y, d.z) * hero->runSpeed * delta);
	btTransform transform = character->getGhostObject()->getWorldTransform();

	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, hero->turnAngle, glm::vec3(0, 1, 0));
	hero->setModel(model);
}

void WorldPhysics::characterDoActionJump(std::shared_ptr<Character> hero, bool forward, float delta)
{
	if(forward)
	{
		glm::vec3 & d{hero->walkDirection};
		character->setWalkDirection(btVector3(d.x, d.y, d.z) * hero->runSpeed * delta);
	}
	if(character->canJump())
	{
		character->jump(btVector3(0.0f, 12.5f, 0.0f));
	}
	btTransform transform = character->getGhostObject()->getWorldTransform();
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, hero->turnAngle, glm::vec3(0, 1, 0));
	hero->setModel(model);
}

void WorldPhysics::characterDoActionIdle(std::shared_ptr<Character> hero)
{
	character->setWalkDirection(btVector3(0, 0, 0));
	btTransform transform = character->getGhostObject()->getWorldTransform();
	
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, hero->turnAngle, glm::vec3(0, 1, 0));
	hero->setModel(model);
}

std::shared_ptr<Vehicle> WorldPhysics::addVehicle(
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
		std::vector<std::shared_ptr<Object>> wheel,
		glm::vec3 upVector)
{
	// vehicle data
	std::shared_ptr<Vehicle> vehicle{std::make_shared<Vehicle>(drive_steer_brake, sDamping, sStiffness, sCompression, sRestLength, rollInfluence, wheel_width, wheel_friction, wheel_radius, wheel, upVector)};

	vehicle->raycaster = new btDefaultVehicleRaycaster(dynamicsWorld);
	rigidBodies[rbChassisIndex]->setActivationState(DISABLE_DEACTIVATION);
	vehicle->vehicle = new btRaycastVehicle(vehicle->tuning, rigidBodies[rbChassisIndex], vehicle->raycaster);
	vehicle->vehicle->setCoordinateSystem(0, 1, 2);
	dynamicsWorld->addVehicle(vehicle->vehicle);

	// set wheels
	btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
	vehicle->setWheels(wheelDirection, wheelAxis, connectionPoint, frontWheel);

	// done
	return vehicle;
}

void WorldPhysics::vehicleDrive(std::shared_ptr<Vehicle> & v, bool forward)
{
	if(forward)
	{
		v->data.engineForce += v->data.engineIncrement;
		v->data.engineForce = std::min(v->data.engineForce, v->data.maxEngineForce);
	}
	else
	{
		v->data.engineForce -= v->data.engineIncrement;
		v->data.engineForce = std::max(v->data.engineForce, 0.0f);
		v->data.brakeForce += v->data.brakeIncrement;
		v->data.brakeForce = std::min(v->data.brakeForce, v->data.maxBrakeForce);
		for(int i{0}; i < 4; ++i)
		{
			if(!v->data.front[i])
				v->vehicle->setBrake(v->data.brakeForce, i);
		}
	}
	for(int i{0}; i < 4; ++i)
	{
		if(!v->data.front[i])
			v->vehicle->applyEngineForce(v->data.engineForce, i);
	}
}

void WorldPhysics::vehicleDriveReset(std::shared_ptr<Vehicle> & v)
{
	v->data.engineForce -= v->data.engineIncrement / 2.0f;
	v->data.engineForce = std::max(v->data.engineForce, 0.0f);
	v->data.brakeForce += v->data.brakeIncrement;
	v->data.brakeForce = std::min(v->data.brakeForce, v->data.maxBrakeForce);
	for(int i{0}; i < 4; ++i)
	{
		if(!v->data.front[i])
			v->vehicle->setBrake(v->data.brakeForce, i);
	}
	for(int i{0}; i < 4; ++i)
	{
		if(!v->data.front[i])
			v->vehicle->applyEngineForce(v->data.engineForce, i);
	}
}

void WorldPhysics::vehicleSteering(std::shared_ptr<Vehicle> & v, VEHICLE_STEERING dir)
{
	if(dir == VEHICLE_STEERING::RIGHT)
	{
		v->data.steering -= v->data.steeringIncrement;
		v->data.steering = std::max(v->data.steering, -v->data.maxSteeringForce);
	}
	else if(dir == VEHICLE_STEERING::LEFT)
	{
		v->data.steering += v->data.steeringIncrement;
		v->data.steering = std::min(v->data.steering, v->data.maxSteeringForce);
	}
	for(int i{0}; i < 4; ++i)
	{
		if(v->data.front[i])
			v->vehicle->setSteeringValue(v->data.steering, i);
	}
}

void WorldPhysics::vehicleSteeringReset(std::shared_ptr<Vehicle> & v)
{
	if(v->data.steering < 0.0f)
	{
		v->data.steering += v->data.steeringIncrement;
		v->data.steering = std::min(v->data.steering, v->data.maxSteeringForce);
	}
	else if(v->data.steering > 0.0f)
	{
		v->data.steering -= v->data.steeringIncrement;
		v->data.steering = std::max(v->data.steering, -v->data.maxSteeringForce);
	}
	for(int i{0}; i < 4; ++i)
	{
		if(v->data.front[i])
			v->vehicle->setSteeringValue(v->data.steering, i);
	}
}

void WorldPhysics::setVehicleWheelTransform(std::shared_ptr<Vehicle> & v)
{
	for(int i{0}; i < 4; ++i)
	{
		btTransform tr{v->vehicle->getWheelTransformWS(i)};
		float oglMatrix[16];
		tr.getOpenGLMatrix(oglMatrix);
		glm::mat4 model(
				oglMatrix[0], oglMatrix[1], oglMatrix[2], oglMatrix[3],
				oglMatrix[4], oglMatrix[5], oglMatrix[6], oglMatrix[7],
				oglMatrix[8], oglMatrix[9], oglMatrix[10], oglMatrix[11],
				oglMatrix[12], oglMatrix[13], oglMatrix[14], oglMatrix[15]
				);
		v->data.wheel[i]->setModel(model);
	}
}

void WorldPhysics::updateVehicleUpVector(std::shared_ptr<Vehicle> & v)
{
	btTransform tr{v->vehicle->getChassisWorldTransform()};
	btQuaternion btQuat = tr.getRotation();
	btScalar angle = std::floor(btQuat.getAngle() * 10.0f) / 10.0f;
	btVector3 axis = btQuat.getAxis();
	glm::vec3 up = glm::rotate(v->baseUp, angle, glm::vec3(axis.x(), axis.y(), axis.z()));
	v->up = glm::normalize(up);
}

void WorldPhysics::addSliderConstraint(int rbIndexA, int rbIndexB, float lowerLinLimit, float upperLinLimit, float lowerAngLimit, float upperAngLimit)
{
    btRigidBody * rbA = rigidBodies[rbIndexA];
    btRigidBody * rbB = rigidBodies[rbIndexB];
    rbA->setActivationState(DISABLE_DEACTIVATION);
    rbB->setActivationState(DISABLE_DEACTIVATION);

    btTransform trA, trB;
    trA = btTransform::getIdentity();
    trB = btTransform::getIdentity();

    btSliderConstraint * constraint = new btSliderConstraint(*rbA, *rbB, trA, trB, true);
    constraint->setLowerLinLimit(lowerLinLimit);
    constraint->setUpperLinLimit(upperLinLimit);
    constraint->setLowerAngLimit(lowerAngLimit);
    constraint->setUpperAngLimit(upperAngLimit);

    dynamicsWorld->addConstraint(constraint);
}

void WorldPhysics::applyImpulse(int rbIndex, btVector3 impulse, btVector3 relPos)
{
    btRigidBody * rb = rigidBodies[rbIndex];
    rb->applyImpulse(impulse, relPos);
}

float WorldPhysics::getDistance(btVector3 from, btVector3 to)
{
    btCollisionWorld::ClosestRayResultCallback firstHit(from, to);
    firstHit.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
    dynamicsWorld->rayTest(from, to, firstHit);
    if(firstHit.hasHit())
    {
        btVector3 hit = from.lerp(to, firstHit.m_closestHitFraction);
        return (hit - from).length();
    }
    return 0.0f;
}

// ######################################################################
// ######################################################################
// ######################################################################

btDebugDraw::btDebugDraw() :
	shader("shaders/axis/vertex.glsl", "shaders/axis/fragment.glsl")
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
