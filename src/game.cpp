#include "game.hpp"

Game::Game(int clientWidth, int clientHeight) :
	activeScene(0),
	activeVehicle(-1),
	graphics(std::make_unique<Graphics>(clientWidth, clientHeight))
{
	// instance models
	std::vector<glm::mat4> pillarInstance;
	pillarInstance.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-10.402f, 4.0f, -5.6927f)));
	pillarInstance.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5.7904f, 4.0f, -10.304f)));

	// skybox textures
	std::vector<std::string> skyTextures;
	skyTextures.push_back("assets/skyboxes/skybox5/px.png");
	skyTextures.push_back("assets/skyboxes/skybox5/nx.png");
	skyTextures.push_back("assets/skyboxes/skybox5/py.png");
	skyTextures.push_back("assets/skyboxes/skybox5/ny.png");
	skyTextures.push_back("assets/skyboxes/skybox5/pz.png");
	skyTextures.push_back("assets/skyboxes/skybox5/nz.png");
	
	// scene objects
	std::vector<std::shared_ptr<Object>> scene_objects;

	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	glm::vec3 camRight;
	glm::vec3 camUp;
/*
	// create test scene
	scenes.push_back(std::make_shared<Scene>("test scene", 0));

	camPos = glm::vec3(-3.792668f, 10.760394f, 13.220017f);
	camTarget = glm::vec3(0.0f, 4.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);
	
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addPointLight(SHADOW_QUALITY::HIGH, glm::vec3(-3.5f, 2.0f, -5.75f), glm::vec3(0.025f), glm::vec3(1.0f, 0.9f, 0.6f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);

	scenes[scenes.size()-1]->addObject("assets/character/ground.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("assets/character/campfire.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("assets/character/ball.glb", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, -4.0f)));
	scenes[scenes.size()-1]->addObject("assets/character/ball2.glb", glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 4.0f, -2.0f)));
	scenes[scenes.size()-1]->addObject("assets/character/bench.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("assets/character/bench.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("assets/character/pillar.glb", glm::mat4(1.0f), "assets/character/pillar_collision_shape.glb", pillarInstance);
	scenes[scenes.size()-1]->addObject("assets/character/flag.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("assets/character/flag_bearer.glb", glm::mat4(1.0f));

	scenes[scenes.size()-1]->setIBL("assets/HDRIs/sky_night_red.hdr", true, clientWidth, clientHeight);
	scenes[scenes.size()-1]->setGridAxis(8);

	scenes[scenes.size()-1]->addParticlesEmitter(glm::vec3(-3.5f, 0.5f, -5.75f), 20, 5.0f, ParticleEmitter::DIRECTION::VECTOR, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// set physics properties for scene
	worldPhysics.push_back(std::make_unique<WorldPhysics>());
	scene_objects = scenes[scenes.size()-1]->getObjects();
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[0], glm::mat4(1.0f), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[1], glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 0.0f, -5.75f)), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::BOX);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[2], glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, -4.0f)), btScalar(0.35), btScalar(0.7), COLLISION_SHAPE::SPHERE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[3], glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 4.0f, -2.0f)), btScalar(0.25), btScalar(0.7), COLLISION_SHAPE::SPHERE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[4], glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 1.5f, 0.0f)), 3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)), btScalar(3.0), btScalar(0.025), COLLISION_SHAPE::BOX);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[5], glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 1.5f, 0.0f)), -3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)), btScalar(3.0), btScalar(0.025), COLLISION_SHAPE::BOX);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[6], glm::mat4(1.0f), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[worldPhysics.size() - 1]->addSoftBody(scene_objects[7], btScalar(0.25));
	worldPhysics[worldPhysics.size() - 1]->attachVertexSoftBody(0, 6, 60);
	worldPhysics[worldPhysics.size() - 1]->attachVertexSoftBody(0, 6, 62);

	// update loadedAssets
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/ground.glb", scene_objects[0]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/campfire.glb", scene_objects[1]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/ball.glb", scene_objects[2]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/ball2.glb", scene_objects[3]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/bench.glb", scene_objects[4]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/bench.glb", scene_objects[5]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/pillar.glb", scene_objects[6]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/flag.glb", scene_objects[7]));
	loadedAssets.insert(std::pair<std::string, std::shared_ptr<Object>>("assets/character/flag_bearer.glb", scene_objects[8]));
*/
/*
	// create car scene
	scenes.push_back(std::make_shared<Scene>("car scene", 0));

	camPos = glm::vec3(-3.792668f, 10.760394f, 13.220017f);
	camTarget = glm::vec3(0.0f, 4.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 50.0f, 0.1f, 500.0f);
	
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addDirectionalLight(SHADOW_QUALITY::ULTRA, glm::vec3(-5.0f, 20.0f, -20.0f), glm::vec3(0.025f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f), glm::vec3(1.0f, -1.5f, -0.15f), 50.0f);
	scenes[scenes.size()-1]->addSpotLight(SHADOW_QUALITY::HIGH, glm::vec3(1.9684f, 2.1581f, -5.6881f), glm::vec3(0.025f), glm::vec3(70.f, 70.f, 70.f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 25.0f, 30.0f);
	scenes[scenes.size()-1]->addSpotLight(SHADOW_QUALITY::HIGH, glm::vec3(-1.9684f, 2.1581f, -5.6881f), glm::vec3(0.025f), glm::vec3(70.f, 70.f, 70.f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 25.0f, 30.0f);

	scenes[0]->addObject("assets/car/ground.glb", glm::mat4(1.0f));
	scenes[0]->addObject("assets/car/chassis.glb", glm::mat4(1.0f), "assets/car/chassis_collision_shape.glb");
	scenes[0]->addObject("assets/car/wheel.glb", glm::mat4(1.0f));
	scenes[0]->addObject("assets/car/wheel.glb", glm::mat4(1.0f));
	scenes[0]->addObject("assets/car/wheel.glb", glm::mat4(1.0f));
	scenes[0]->addObject("assets/car/wheel.glb", glm::mat4(1.0f));

	scenes[scenes.size()-1]->setIBL("assets/HDRIs/sky.hdr", true, clientWidth, clientHeight);
	scenes[scenes.size()-1]->setGridAxis(8);

	// set physics properties for scene
	worldPhysics.push_back(std::make_unique<WorldPhysics>());
	scene_objects = scenes[0]->getObjects();
	worldPhysics[0]->addRigidBody(scene_objects[0], glm::mat4(1.0f), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[0]->addRigidBody(scene_objects[1], glm::mat4(1.0f), btScalar(1.0f), btScalar(0.15f), COLLISION_SHAPE::CONVEX_HULL);
	std::array<float, 6> drive_steer_brake{2.5f, 0.05f, 0.9f, 0.025f, 0.5f, 0.01f};
	std::vector<btVector3> connectionPoint{
		btVector3(2.25f, 1.0f, 3.25f),
		btVector3(-2.25f, 1.0f, 3.25f),
		btVector3(2.25f, 1.0f, -4.0f),
		btVector3(-2.25f, 1.0f, -4.0f)
	};
	std::array<bool, 4> frontWheel{false, false, true, true};
	std::vector<std::shared_ptr<Object>> wheel{scene_objects[2], scene_objects[3], scene_objects[4], scene_objects[5]};
	std::shared_ptr<Vehicle> v = worldPhysics[0]->addVehicle(drive_steer_brake, 5.0f, 7.0f, 0.25f, 0.5f, 2.5f, 0.25f, 2.0f, 0.85f, btVector3(1.0f, 0.0f, 0.0f), connectionPoint, frontWheel, 1, wheel, glm::vec3(0.0f, 1.0f, 0.0f));
	scenes[scenes.size()-1]->addVehicle(v);
*/
/*
	// create emission scene
	scenes.push_back(std::make_shared<Scene>("emission", 0));

	camPos = glm::vec3(-3.792668f, 10.760394f, 13.220017f);
	camTarget = glm::vec3(0.0f, 4.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 50.0f, 0.1f, 1000.0f);
	
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addDirectionalLight(SHADOW_QUALITY::HIGH, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.025f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f), glm::vec3(0.25f, -1.0f, -0.15f), 30.0f);

	scenes[0]->addObject("assets/lightsaber/lightsaber.glb", glm::mat4(1.0f));

	scenes[scenes.size()-1]->setGridAxis(8);
*/
/*
	// create podracer scene
	scenes.push_back(std::make_shared<Scene>("podracer", 0));

	camPos = glm::vec3(-3.792668f, 10.760394f, 13.220017f);
	camTarget = glm::vec3(0.0f, 4.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 50.0f, 0.1f, 1000.0f);
	
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addDirectionalLight(SHADOW_QUALITY::HIGH, glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.025f), glm::vec3(1.5f, 1.2f, 0.9f) * 0.5f, glm::vec3(1.0f), glm::vec3(0.0, -1.0f, -0.5f), 10.0f);

	scenes[0]->addObject("assets/pod/pod.gltf", glm::mat4(1.0f));
	scenes[0]->addObject("assets/pod/ground.gltf", glm::mat4(1.0f));
	
	std::vector<float> arcs{0.1f, 0.01f, 0.18f, 0.01f};
	scenes[0]->addLightning(glm::vec3(0.98188f, 1.5146f, 0.45293f), glm::vec3(-0.98188f, 1.5146f, 0.45293f), 30, glm::vec3(0.79f, 0.017f, 0.8f), 10.0f, arcs, true, 0.25f);

	scenes[scenes.size()-1]->setIBL("assets/HDRIs/bridge.hdr", true, clientWidth, clientHeight);
	scenes[scenes.size()-1]->setGridAxis(8);
*/

	// create audio scene
	scenes.push_back(std::make_shared<Scene>("audio", 0));

	camPos = glm::vec3(0.0f, 5.0f, 5.0f);
	camTarget = glm::vec3(0.0f, 1.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 50.0f, 0.1f, 100.0f);
	
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addPointLight(SHADOW_QUALITY::HIGH, glm::vec3(0.0f, 7.0f, 5.0f), glm::vec3(0.25f), glm::vec3(1.75f, 1.5f, 0.9f), glm::vec3(1.0f), 1.0f, 0.07f, 0.014f);

	scenes[0]->addObject("assets/radio/radio.glb", glm::mat4(1.0f));
	scenes[0]->addAudioFile("assets/radio/cantina.wav");
	scenes[0]->addSoundSource(glm::vec3(-0.2f, 2.4f, 0.5f), glm::vec3(-0.25f, 0.0f, 1.0f), 90.0f, 360.0f, 10.0f, false);
	scenes[0]->playSound(0, 0);

	scenes[scenes.size()-1]->setIBL("assets/HDRIs/sky.hdr", true, clientWidth, clientHeight);
	scenes[scenes.size()-1]->setGridAxis(20);
}

void Game::draw(float& delta, int width, int height, DRAWING_MODE mode, bool debug, bool debugPhysics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update physics
	if(!worldPhysics.empty())
	{
		if(debugPhysics)
		{
			worldPhysics[activeScene]->stepSimulation(
					scenes[activeScene]->getActiveCamera()->getViewMatrix(),
					scenes[activeScene]->getActiveCamera()->getProjectionMatrix()
					);
			return;
		}
		worldPhysics[activeScene]->stepSimulation();
	}

	// update character's current animation
	if(character && character->sceneID == scenes[activeScene]->getId())
	{
		character->get()->getAnimator()->updateAnimation(delta);
	}

	// update vehicle spotlights
	/*
	glm::mat4 vehicleModel;
	scenes[0]->getVehicles()[0]->vehicle->getRigidBody()->getCenterOfMassTransform().getOpenGLMatrix(glm::value_ptr(vehicleModel));
	glm::vec3 spot1 = glm::vec3(vehicleModel * glm::vec4(1.9684f, 2.1581f, -5.6881f, 1.0f));
	glm::vec3 spot2 = glm::vec3(vehicleModel * glm::vec4(-1.9684f, 2.1581f, -5.6881f, 1.0f));
	glm::vec3 d1 = glm::vec3(vehicleModel * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	scenes[activeScene]->getSLights()[0]->setPosition(spot1);
	scenes[activeScene]->getSLights()[0]->setDirection(d1);
	scenes[activeScene]->getSLights()[1]->setPosition(spot2);
	scenes[activeScene]->getSLights()[1]->setDirection(d1);
	*/
	// get shader
	Shader s = graphics->getPBRShader();
	//Shader s = graphics->getBlinnPhongShader();

	if(activeScene < scenes.size())
	{
		if(graphics->shadowsOn())
		{
			s.use();
			s.setInt("shadowOn", 1);

			// SHADOW PASS : directional & spot light sources
			directionalShadowPass(activeScene, delta, mode);
			
			// SHADOW PASS : point light sources
			omnidirectionalShadowPass(activeScene, delta, mode);

			// SSAO PASS
			if(graphics->ssaoOn())
				ssaoPass(activeScene, width, height, delta);

			// COLOR PASS : multisampling
			colorMultisamplePass(activeScene, width, height, delta, mode, debug);

			// blit to normal framebuffer (resolve multisampling)
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			graphics->getMultisampleFBO()->blitFramebuffer(graphics->getNormalFBO(0), width, height);
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			graphics->getMultisampleFBO()->blitFramebuffer(graphics->getNormalFBO(1), width, height);

			// BLOOM PASS
			if(graphics->bloomOn())
				bloomPass(width, height);

			// VOLUMETRICS PASS
			if(graphics->volumetricLightingOn())
				volumetricsPass(activeScene, width, height, delta);

			// BIND TO DEFAULT FRAMEBUFFER
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// DRAW FINAL IMAGE QUAD
			graphics->getFinalShader().use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments()[0].id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getUpSamplingFBO(11)->getAttachments()[0].id);
				graphics->getFinalShader().setInt("bloom", 1);
				graphics->getFinalShader().setInt("bloomEffect", 1);
			}
			else
			{
				graphics->getFinalShader().setInt("bloomEffect", 0);
			}
			graphics->getFinalShader().setInt("tone_mapping", static_cast<int>(graphics->get_tone_mapping()));
			graphics->getQuadMesh()->draw(graphics->getFinalShader());
		}
		else
		{
			s.use();
			s.setInt("shadowOn", 0);

			// SSAO PASS
			if(graphics->ssaoOn())
				ssaoPass(activeScene, width, height, delta);

			// render to multisample framebuffer
			s.use();
			glViewport(0, 0, width, height);
			graphics->getMultisampleFBO()->bind();

			// draw scene
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			s.setVec3f("cam.viewPos", scenes[activeScene]->getActiveCamera()->getPosition());
			s.setMatrix("view", scenes[activeScene]->getActiveCamera()->getViewMatrix());
			s.setMatrix("proj", scenes[activeScene]->getActiveCamera()->getProjectionMatrix());
			s.setLighting(scenes[activeScene]->getPLights(), scenes[activeScene]->getDLights(), scenes[activeScene]->getSLights());
			scenes[activeScene]->draw(s, graphics, delta, mode, debug);
			
			// blit to normal framebuffer (resolve multisampling)
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			graphics->getMultisampleFBO()->blitFramebuffer(graphics->getNormalFBO(0), width, height);
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			graphics->getMultisampleFBO()->blitFramebuffer(graphics->getNormalFBO(1), width, height);

			// BLOOM PASS
			if(graphics->bloomOn())
				bloomPass(width, height);

			// VOLUMETRICS PASS
			if(graphics->volumetricLightingOn())
				volumetricsPass(activeScene, width, height, delta);

			// bind to default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw final image quad
			graphics->getFinalShader().use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments()[0].id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getUpSamplingFBO(11)->getAttachments()[0].id);
				graphics->getFinalShader().setInt("bloom", 1);
				graphics->getFinalShader().setInt("bloomEffect", 1);
			}
			else
			{
				graphics->getFinalShader().setInt("bloomEffect", 0);
			}
			graphics->getFinalShader().setInt("tone_mapping", static_cast<int>(graphics->get_tone_mapping()));
			graphics->getQuadMesh()->draw(graphics->getFinalShader());
		}
	}
	else
	{
		std::cerr << "Error: wrong scene index supplied for draw command.\n";
	}
}

void Game::resizeScreen(int clientWidth, int clientHeight)
{
	#pragma omp parallel for
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i]->updateCameraPerspective(glm::ivec2(clientWidth, clientHeight));
	}

	graphics->resizeScreen(clientWidth, clientHeight);
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	if(index < scenes.size())
	{
		CAM_TYPE type = scenes[index]->getActiveCamera()->getType();
		if(type == CAM_TYPE::REGULAR)
			scenes[index]->getActiveCamera()->updateViewMatrix(inputs, mouse, delta);
		else if(type == CAM_TYPE::THIRD_PERSON)
			scenes[index]->getActiveCamera()->updateViewMatrix(character->getPosition(), character->getDirection(), inputs, mouse, delta);
		else if(type == CAM_TYPE::VEHICLE)
		{
			std::shared_ptr<Vehicle> & vehicle{scenes[index]->getVehicles()[activeVehicle]};
			btRaycastVehicle * raycastVehicle = vehicle->vehicle;
			btVector3 position = raycastVehicle->getChassisWorldTransform().getOrigin();
			glm::vec3 pos(position.x(), position.y(), position.z());
			btVector3 direction = raycastVehicle->getForwardVector();
			glm::vec3 dir(direction.x(), direction.y(), direction.z());
			float steerAngle = vehicle->data.steering;
			glm::vec3 up = vehicle->up;
			scenes[index]->getActiveCamera()->updateViewMatrix(pos, -dir, up, -steerAngle, vehicle->data.steeringIncrement, inputs, mouse, delta);
		}
	}
}

std::unique_ptr<Graphics> & Game::getGraphics()
{
	return graphics;
}

std::vector<std::shared_ptr<Scene>> & Game::getScenes()
{
	return scenes;
}

int Game::getActiveScene()
{
	return activeScene;
}

void Game::setActiveScene(int index)
{
	activeScene = index;
}

int Game::getActiveVehicle()
{
	return activeVehicle;
}

void Game::setActiveVehicle(int index)
{
	activeVehicle = index;
}

void Game::setCharacterScene(int index)
{
	if(character)
	{
		character->sceneID = index;
	}
}

int Game::getCharacterScene()
{
	if(character)
	{
		return character->sceneID;
	}
	return -1;
}

void Game::setCharacter(std::string filePath, glm::mat4 aModel, std::string aName, int index, glm::ivec2 scrDim)
{
	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	glm::vec3 camRight;
	glm::vec3 camUp;

	character = std::make_shared<Character>(filePath, aModel, aName);
	setCharacterScene(index);

	camTarget = character->getPosition();
	camDir = glm::normalize(character->getDirection());
	camPos = camTarget - (camDir * 7.0f) + glm::vec3(0.0f, 3.0f, 0.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[character->sceneID]->addCamera(CAM_TYPE::THIRD_PERSON, scrDim, camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);
	scenes[character->sceneID]->setActiveCamera(scenes[character->sceneID]->getCameras().size() - 1);

	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i]->setCharacter(character);
		worldPhysics[i]->setKinematicCharacter(character->get(), character->getModel());
	}
}

void Game::removeCharacter()
{
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i]->removeCharacter();
		worldPhysics[i]->removeKinematicCharacter();
	}
	character.reset();
}

void Game::characterDoActionWalk(Character::DIRECTION d, float delta)
{
	if(character)
	{
		character->walk();
		worldPhysics[activeScene]->characterDoActionWalk(character, d, delta);
	}
}

void Game::characterDoActionRun(Character::DIRECTION d, float delta)
{
	if(character)
	{
		character->run();
		worldPhysics[activeScene]->characterDoActionRun(character, d, delta);
	}
}

void Game::characterDoActionJump(bool forward, float delta)
{
	if(character)
	{
		character->jump();
		worldPhysics[activeScene]->characterDoActionJump(character, forward, delta);
	}
}

void Game::characterDoActionIdle()
{
	if(character)
	{
		character->idle();
		worldPhysics[activeScene]->characterDoActionIdle(character);
	}
}

void Game::vehicleDrive(bool forward)
{
	worldPhysics[activeScene]->vehicleDrive(scenes[activeScene]->getVehicles()[activeVehicle], forward);
}

void Game::vehicleDriveReset()
{
	worldPhysics[activeScene]->vehicleDriveReset(scenes[activeScene]->getVehicles()[activeVehicle]);
}

void Game::vehicleSteering(VEHICLE_STEERING dir)
{
	worldPhysics[activeScene]->vehicleSteering(scenes[activeScene]->getVehicles()[activeVehicle], dir);
}

void Game::vehicleSteeringReset()
{
	worldPhysics[activeScene]->vehicleSteeringReset(scenes[activeScene]->getVehicles()[activeVehicle]);
}

void Game::vehicleSetWheelTransform()
{
	worldPhysics[activeScene]->setVehicleWheelTransform(scenes[activeScene]->getVehicles()[activeVehicle]);
}

void Game::vehicleUpdateUpVector()
{
	worldPhysics[activeScene]->updateVehicleUpVector(scenes[activeScene]->getVehicles()[activeVehicle]);
}

void Game::directionalShadowPass(int index, float delta, DRAWING_MODE mode)
{
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 0);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 0);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 0);

	// render directional depth maps
	int sLightsOffset{0};
	for(int i{0}; i < scenes[index]->getDLights().size(); ++i, ++sLightsOffset)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index]->getDLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index]->getDLights()[i]->getShadowQuality()));
		graphics->setStdShadowQuality(scenes[index]->getDLights()[i]->getShadowQuality(), i);
		graphics->getStdDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes[index]->getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		graphics->getShadowMappingShader().setMatrix("view", lightView);
		graphics->getShadowMappingShader().setMatrix("proj", graphics->getOrthoProjection(scenes[index]->getDLights()[i]->getOrthoDimension()));

		// draw scene
		scenes[index]->draw(graphics->getShadowMappingShader(), graphics, delta, mode);
	}

	for(int i{0}; i < scenes[index]->getSLights().size(); ++i)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index]->getSLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index]->getSLights()[i]->getShadowQuality()));
		graphics->setStdShadowQuality(scenes[index]->getSLights()[i]->getShadowQuality(), i + scenes[index]->getDLights().size());
		graphics->getStdDepthFBO(sLightsOffset + i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes[index]->getSLights()[i]->getPosition();
		glm::vec3 lightDirection = scenes[index]->getSLights()[i]->getDirection();
		glm::vec3 lightTarget = lightPosition + lightDirection;
		glm::vec3 up = (lightDirection == glm::vec3(0.0f, -1.0f, 0.0f)) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, up);
		float outerCutOff = scenes[index]->getSLights()[i]->getOuterCutOff();

		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes[index]->getActiveCamera()->getNearPlane(),
					scenes[index]->getActiveCamera()->getFarPlane()
					);

		graphics->getShadowMappingShader().setMatrix("proj", spotProj);
		graphics->getShadowMappingShader().setMatrix("view", lightView);

		// draw scene
		scenes[index]->draw(graphics->getShadowMappingShader(), graphics, delta, mode);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::omnidirectionalShadowPass(int index, float delta, DRAWING_MODE mode)
{
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 1);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 1);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 1);

	// render omnidirectional depth maps
	std::vector<glm::mat4> omnilightViews;
	for(int i{0}; i < scenes[index]->getPLights().size(); ++i)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index]->getPLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index]->getPLights()[i]->getShadowQuality()));
		graphics->setOmniShadowQuality(scenes[index]->getPLights()[i]->getShadowQuality(), i);
		graphics->getOmniDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		// (proj * view)
		glm::vec3 lightPosition = scenes[index]->getPLights()[i]->getPosition();
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		graphics->getShadowMappingShader().setVec3f("lightPosition", lightPosition);
		for(int j{0}; j < 6; ++j)
			graphics->getShadowMappingShader().setMatrix("omnilightViews[" + std::to_string(j) + "]", omnilightViews[j]);
		omnilightViews.clear();

		// draw scene
		scenes[index]->draw(graphics->getShadowMappingShader(), graphics, delta, mode);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::colorMultisamplePass(int index, int width, int height, float delta, DRAWING_MODE mode, bool debug)
{
	// render to multisample framebuffer
	glViewport(0, 0, width, height);
	graphics->getMultisampleFBO()->bind();

	// get shader
	Shader s = graphics->getPBRShader();
	//Shader s = graphics->getBlinnPhongShader();

	// draw scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s.use();
	s.setInt("pointLightCount", scenes[index]->getPLights().size());
	s.setVec3f("cam.viewPos", scenes[index]->getActiveCamera()->getPosition());
	s.setMatrix("view", scenes[index]->getActiveCamera()->getViewMatrix());
	s.setMatrix("proj", scenes[index]->getActiveCamera()->getProjectionMatrix());
	s.setLighting(scenes[index]->getPLights(), scenes[index]->getDLights(), scenes[index]->getSLights());
	s.setInt("hasSSAO", graphics->ssaoOn() ? 1 : 0);
	glActiveTexture(GL_TEXTURE0 + 14);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(1)->getAttachments()[0].id);
	s.setInt("ssao", 14);
	s.setVec2f("viewport", glm::vec2(width, height));

	// set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes[index]->getPLights().size();
	int nbDLights = scenes[index]->getDLights().size();
	int nbSLights = scenes[index]->getSLights().size();

	int textureOffset{5};

	for(int i{0}; i < nbPLights; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_CUBE_MAP, graphics->getOmniDepthFBO(i)->getAttachments()[0].id);
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i) + "].lightSpaceMatrix", glm::mat4(1.0f));
		textureOffset++;
	}
	// "you have to uniform all elements in samplerCube array. Otherwise, there will be a"
	// "black screen, or your clear color. Also, following draw calls may cause invalid "
	// "operation. Better to uniform all unused sampler types with some random texture index."
	for(int i{nbPLights}; i < 10; ++i)
	{
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", textureOffset);
	}

	int depthMapIndex{0};
	for(int i{0}; i < nbDLights; ++i)
	{
		glm::vec3 lightPosition = scenes[index]->getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(depthMapIndex)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics->getOrthoProjection(scenes[index]->getDLights()[i]->getOrthoDimension()) * lightView);
		depthMapIndex++;
		textureOffset++;
	}

	for(int i{0}; i < nbSLights; ++i)
	{
		float outerCutOff = scenes[index]->getSLights()[i]->getOuterCutOff();
		glm::vec3 lightPosition = scenes[index]->getSLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getSLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes[index]->getActiveCamera()->getNearPlane(),
					scenes[index]->getActiveCamera()->getFarPlane()
					);

		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(depthMapIndex)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i + nbPLights + nbDLights) + "].lightSpaceMatrix", spotProj * lightView);
		depthMapIndex++;
		textureOffset++;
	}

	scenes[index]->draw(s, graphics, delta, mode, debug);
}

void Game::bloomPass(int width, int height)
{
	bool firstIteration{true};
	Shader & downSampling = graphics->getDownSamplingShader();
	Shader & upSampling = graphics->getUpSamplingShader();
	Shader & gaussianBlur = graphics->getGaussianBlurShader();
	Shader & tentBlur = graphics->getTentBlurShader();
	
	// downsampling and blurring
	for(int i{0}; i < 6; ++i)
	{
		downSampling.use();
		int factor = std::pow(2, i+1);
		glViewport(0, 0, width / factor, height / factor);
		std::unique_ptr<Framebuffer> & fbo = graphics->getDownSamplingFBO(i);
		fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		downSampling.setInt("image", 0);
		glActiveTexture(GL_TEXTURE0);
		if(firstIteration)
		{
			firstIteration = false;
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(1)->getAttachments()[0].id);
		}
		else
			glBindTexture(GL_TEXTURE_2D, graphics->getPingPongFBO(i*2-1)->getAttachments()[0].id);
		graphics->getQuadMesh()->draw(downSampling);

		// apply horizontal gaussian blur
		gaussianBlur.use();
		std::unique_ptr<Framebuffer> & ping = graphics->getPingPongFBO(i*2);
		ping->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		gaussianBlur.setInt("image", 0);
		gaussianBlur.setInt("blurSize", graphics->getBloomSize());
		gaussianBlur.setFloat("sigma", graphics->getBloomSigma());
		gaussianBlur.setInt("direction", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics->getDownSamplingFBO(i)->getAttachments()[0].id);
		graphics->getQuadMesh()->draw(gaussianBlur);
		
		// apply vertical gaussian blur
		std::unique_ptr<Framebuffer> & pong = graphics->getPingPongFBO(i*2+1);
		pong->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		gaussianBlur.setInt("image", 0);
		gaussianBlur.setInt("blurSize", graphics->getBloomSize());
		gaussianBlur.setFloat("sigma", graphics->getBloomSigma());
		gaussianBlur.setInt("direction", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics->getPingPongFBO(i*2)->getAttachments()[0].id);
		graphics->getQuadMesh()->draw(gaussianBlur);
	}

	// upsampling
	firstIteration = true;
	for(int i{0}; i < 6; ++i)
	{
		upSampling.use();
		int factor = std::pow(2, 5-i);
		glViewport(0, 0, width / factor, height / factor);
		std::unique_ptr<Framebuffer> & mergeFBO = graphics->getUpSamplingFBO(i*2);
		mergeFBO->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		upSampling.setInt("low_res", 0);
		upSampling.setInt("high_res", 1);
		glActiveTexture(GL_TEXTURE0);
		if(firstIteration)
		{
			firstIteration = false;
			glBindTexture(GL_TEXTURE_2D, graphics->getPingPongFBO((5-i)*2+1)->getAttachments()[0].id);
		}
		else
			glBindTexture(GL_TEXTURE_2D, graphics->getUpSamplingFBO((i-1)*2+1)->getAttachments()[0].id);
		glActiveTexture(GL_TEXTURE1);
		if(4-i == -1)
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(1)->getAttachments()[0].id);
		else
			glBindTexture(GL_TEXTURE_2D, graphics->getPingPongFBO((4-i)*2+1)->getAttachments()[0].id);
		graphics->getQuadMesh()->draw(upSampling);

		// apply tent filter
		tentBlur.use();
		std::unique_ptr<Framebuffer> & tentFBO = graphics->getUpSamplingFBO(i*2+1);
		tentFBO->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		tentBlur.setInt("image", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics->getUpSamplingFBO(i*2)->getAttachments()[0].id);
		graphics->getQuadMesh()->draw(tentBlur);
	}
}

void Game::ssaoPass(int index, int width, int height, float delta)
{
	// Fill G BUFFER
	graphics->getGBufferFBO()->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render position, normal, and depth data
	graphics->getGBufferShader().use();
	graphics->getGBufferShader().setMatrix("view", scenes[index]->getActiveCamera()->getViewMatrix());
	graphics->getGBufferShader().setMatrix("proj", scenes[index]->getActiveCamera()->getProjectionMatrix());
	scenes[index]->draw(graphics->getGBufferShader(), graphics, delta);

	// render ambient occlusion data
	graphics->getAOFBO(0)->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	graphics->getAOShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->getGBufferFBO()->getAttachments()[0].id); // position
	graphics->getAOShader().setInt("positionBuffer", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics->getGBufferFBO()->getAttachments()[1].id); // normal
	graphics->getAOShader().setInt("normalBuffer", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, graphics->getAONoiseTexture()); // noise texture
	graphics->getAOShader().setInt("noiseTexture", 2);
	std::vector<glm::vec3> aoKernel{graphics->getAOKernel()};
	for(int i{0}; i < 32; ++i)
		graphics->getAOShader().setVec3f("samples[" + std::to_string(i) + "]", aoKernel[i]);
	graphics->getAOShader().setFloat("radius", 1.0f);
	graphics->getAOShader().setFloat("bias", 0.05f);
	graphics->getAOShader().setMatrix("projection", scenes[index]->getActiveCamera()->getProjectionMatrix());
	graphics->getAOShader().setFloat("screenWidth", static_cast<float>(width));
	graphics->getAOShader().setFloat("screenHeight", static_cast<float>(height));
	graphics->getQuadMesh()->draw(graphics->getAOShader());

	graphics->getAOFBO(1)->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	graphics->getAOBlurShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(0)->getAttachments()[0].id); // raw AO
	graphics->getAOBlurShader().setInt("aoInput", 0);
	graphics->getQuadMesh()->draw(graphics->getAOBlurShader());

	// reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}

void Game::volumetricsPass(int index, int width, int height, float delta)
{
	graphics->getVolumetricsFBO()->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// get shader
	Shader & s = graphics->getVolumetricLightingShader();
	
	// set shader data
	s.use();
	s.setInt("pointLightCount", scenes[index]->getPLights().size());
	s.setVec3f("cam.viewPos", scenes[index]->getActiveCamera()->getPosition());
	s.setFloat("cam.near_plane", scenes[index]->getActiveCamera()->getNearPlane());
	s.setFloat("cam.far_plane", scenes[index]->getActiveCamera()->getFarPlane());
	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_2D, graphics->getGBufferFBO()->getAttachments()[0].id); // position of each fragment
	s.setInt("fragPosition", 10);
	s.setMatrix("cam.inv_view", glm::inverse(scenes[index]->getActiveCamera()->getViewMatrix()));
	s.setMatrix("cam.inv_proj", glm::inverse(scenes[index]->getActiveCamera()->getProjectionMatrix()));
	s.setLighting(scenes[index]->getPLights(), scenes[index]->getDLights(), scenes[index]->getSLights());

	// set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes[index]->getPLights().size();
	int nbDLights = scenes[index]->getDLights().size();
	int nbSLights = scenes[index]->getSLights().size();

	for(int i{0}; i < nbPLights; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, graphics->getOmniDepthFBO(i)->getAttachments()[0].id);
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", i);
		s.setMatrix("light[" + std::to_string(i) + "].lightSpaceMatrix", glm::mat4(1.0f));
	}
	// "you have to uniform all elements in samplerCube array. Otherwise, there will be a"
	// "black screen, or your clear color. Also, following draw calls may cause invalid "
	// "operation. Better to uniform all unused sampler types with some random texture index."
	for(int i{nbPLights}; i < 10; ++i)
	{
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", nbPLights);
	}

	for(int i{0}; i < nbDLights; ++i)
	{
		glm::vec3 lightPosition = scenes[index]->getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0 + nbPLights + i);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(i)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(i) + "]", nbPLights + i);
		s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics->getOrthoProjection(scenes[index]->getDLights()[i]->getOrthoDimension()) * lightView);
	}

	for(int i{0}; i < nbSLights; ++i)
	{
		float outerCutOff = scenes[index]->getSLights()[i]->getOuterCutOff();
		glm::vec3 lightPosition = scenes[index]->getSLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getSLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes[index]->getActiveCamera()->getNearPlane(),
					scenes[index]->getActiveCamera()->getFarPlane()
					);

		glActiveTexture(GL_TEXTURE0 + nbPLights + nbDLights + i);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(nbDLights + i)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(nbDLights + i) + "]", nbPLights + nbDLights + i);
		s.setMatrix("light[" + std::to_string(i + nbPLights + nbDLights) + "].lightSpaceMatrix", spotProj * lightView);
	}
	
	graphics->getQuadMesh()->draw(s);

	// reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}
