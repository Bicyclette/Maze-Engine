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
}

void WorldPhysics::addSoftBody(std::shared_ptr<Object> object, btScalar mass)
{
	// ########## LAMBDAS >>>>>>>>>>
	std::function getVertexDefinedIndex =
	[] (glm::vec3 pos, int index, const std::vector<std::pair<glm::vec3, int>> & couples){
		for(auto couple : couples)
		{
			glm::vec3 p = couple.first;
			if(p.x == pos.x && p.y == pos.y && p.z == pos.z)
				return couple.second;
		}
		return -1;
	};
	
	std::function fetchCorrectIndex =
	[] (glm::vec3 pos, const std::vector<std::pair<glm::vec3, int>> & couples){
		int index{-1};
		for(auto couple : couples)
		{
			glm::vec3 p = couple.first;
			if(p.x == pos.x && p.y == pos.y && p.z == pos.z)
				return couple.second;
		}
		return -1;
	};

	std::function computeInitialVertexSet =
	[] (std::vector<Vertex> verticesRef, std::vector<glm::vec3> vertices){
		std::vector<Vertex> res;
		for(auto v : vertices)
		{
			for(auto vref : verticesRef)
			{
				glm::vec3 pos = vref.position;
				if(v.x == pos.x && v.y == pos.y && v.z == pos.z)
					res.push_back(vref);
			}
		}
		return res;
	};
	// ########## LAMBDAS <<<<<<<<<<

	std::vector<std::shared_ptr<Mesh>> meshes = object->getMeshes();
	std::shared_ptr<Mesh> mesh = meshes[0];
	std::vector<Vertex> const & vertices = mesh->getVertices();
	std::vector<int> const & indices = mesh->getIndices();

	// get rid of duplicated vertices
	std::vector<glm::vec3> clean_vertex_list;
	std::vector<int> clean_index_list;
	std::vector<std::pair<glm::vec3, int>> couples;

	for(int j{0}; j < indices.size(); ++j)
	{
		//get the 3 couples of the current face
		glm::vec3 pos[3] = {
			vertices[indices[j]].position,
			vertices[indices[j+1]].position,
			vertices[indices[j+2]].position
		};
		int index[3] = {
			indices[j],
			indices[j+1],
			indices[j+2]
		};
		// check for each couple if its position is already defined
		// by another couple in the clean array
		int defined[3];
		defined[0] = getVertexDefinedIndex(pos[0], index[0], couples);
		defined[1] = getVertexDefinedIndex(pos[1], index[1], couples);
		defined[2] = getVertexDefinedIndex(pos[2], index[2], couples);

		for(int k{0}; k < 3; ++k)
			if(defined[k] == -1) { couples.push_back(std::make_pair(pos[0], index[0])); };
	}

	for(int j{0}; j < couples.size(); ++j)
	{
		int id{couples[j].second};
		clean_vertex_list.push_back(vertices[id].position);
	}

	for(int j{0}; j < indices.size(); ++j)
	{
		glm::vec3 pos[3] = {
			vertices[indices[j]].position,
			vertices[indices[j+1]].position,
			vertices[indices[j+2]].position
		};
		for(int k{0}; k < 3; ++k)
			clean_index_list.push_back(fetchCorrectIndex(pos[k], couples));
	}

	// transform mesh's vertex list to a btScalar array with only position info
	int numTriangles{static_cast<int>(clean_index_list.size()) / 3};
	std::unique_ptr<btScalar[]> v{std::make_unique<btScalar[]>(clean_vertex_list.size() * 3)};

	for(int j{0}; j < clean_vertex_list.size(); ++j)
	{
		glm::vec3 pos = clean_vertex_list[j];
		v[j*3] = pos.x;
		v[j*3 + 1] = pos.y;
		v[j*3 + 2] = pos.z;
	}

	// create the soft body
	btSoftBody * softBody = btSoftBodyHelpers::CreateFromTriMesh(
			*softBodyWorldInfo,
			v.get(),
			clean_index_list.data(),
			numTriangles);

	// define soft body material
	btSoftBody::Material * material = softBody->appendMaterial();
	material->m_kLST = 0.75f;
	material->m_kAST = 0.0f;
	material->m_kVST = 0.75f;

	//btSoftBody::Config material_cfg;
	//material_cfg.aeromodel = btSoftBody::eAeroModel::v_Point;
	//softBody->m_cfg = material_cfg;

	softBody->generateBendingConstraints(2, material);
	softBody->generateClusters(128);

	//softBody->m_materials[0]->m_kLST = 0.45f;
	//softBody->m_cfg.kVC = 0.01;
	softBody->setPose(true, false);
	softBody->setTotalMass(mass, false);
	//softBody->generateBendingConstraints(2);
	//softBody->setTotalMass(mass, true);
	softBody->m_cfg.piterations = 2;
	//softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
	softBody->randomizeConstraints();

	dynamicsWorld->addSoftBody(softBody);

	// create initial set of vertex for the mesh
	std::vector<Vertex> initialVertices{computeInitialVertexSet(vertices, clean_vertex_list)};
	mesh->recreate(initialVertices, clean_index_list, true);
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

void WorldPhysics::addKinematicCharacter(std::shared_ptr<Object> object)
{
	struct AABB aabb = object->getAABB();
	float xExtent = aabb.xMax - aabb.xMin;
	float yExtent = aabb.yMax - aabb.yMin;
	float zExtent = aabb.zMax - aabb.zMin;
	
	// main character
	btCapsuleShape * convexShape = new btCapsuleShapeZ(1, 2);
	btPairCachingGhostObject * ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(btTransform(btQuaternion(0,0,0,1), btVector3(0, 5, 0)));
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(convexShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	mainCharacter = new btKinematicCharacterController(ghostObject, convexShape, btScalar(0.25));
	mainCharacter->setUp(btVector3(0,1,0));
	dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(mainCharacter);

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

int WorldPhysics::getNumSoftBody()
{
	return dynamicsWorld->getSoftBodyArray().size();
}

static glm::vec3 d = glm::vec3(0, 0, 1);
static glm::mat4 model;
static float turn = 0.0f;
glm::mat4 WorldPhysics::mainCharacterDoActionWalk(CHARACTER_DIRECTION direction, float delta)
{
	static float speed = 2.5f;
	static float turnSpeed = 0.02f;

	if(direction == CHARACTER_DIRECTION::RIGHT)
	{
		turn += turnSpeed;
		d = glm::rotate(d, turnSpeed, glm::vec3(0, 1, 0));
	}
	else if(direction == CHARACTER_DIRECTION::LEFT)
	{
		turn += -turnSpeed;
		d = glm::rotate(d, -turnSpeed, glm::vec3(0, 1, 0));
	}

	mainCharacter->setWalkDirection(btVector3(d.x, d.y, d.z) * speed * delta);
	btTransform transform = mainCharacter->getGhostObject()->getWorldTransform();
	
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, turn, glm::vec3(0, 1, 0));
	return model;
}

glm::mat4 WorldPhysics::mainCharacterDoActionRun(CHARACTER_DIRECTION direction, float delta)
{
	static float speed = 7.0f;
	static float turnSpeed = 0.07f;

	if(direction == CHARACTER_DIRECTION::RIGHT)
	{
		turn += turnSpeed;
		d = glm::rotate(d, turnSpeed, glm::vec3(0, 1, 0));
	}
	else if(direction == CHARACTER_DIRECTION::LEFT)
	{
		turn += -turnSpeed;
		d = glm::rotate(d, -turnSpeed, glm::vec3(0, 1, 0));
	}

	mainCharacter->setWalkDirection(btVector3(d.x, d.y, d.z) * speed * delta);
	btTransform transform = mainCharacter->getGhostObject()->getWorldTransform();
	
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, turn, glm::vec3(0, 1, 0));
	return model;
}

glm::mat4 WorldPhysics::mainCharacterDoActionIdle()
{
	mainCharacter->setWalkDirection(btVector3(0, 0, 0));
	btTransform transform = mainCharacter->getGhostObject()->getWorldTransform();
	
	btVector3 origin = transform.getOrigin();
	glm::vec3 pos = glm::vec3(origin.getX(), origin.getY() - 2, origin.getZ());
	model = glm::translate(glm::mat4(1.0f), pos);
	model = glm::rotate(model, turn, glm::vec3(0, 1, 0));
	return model;
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
