#include "game.hpp"

Game::Game(int clientWidth, int clientHeight) :
	activeScene(0),
	graphics(std::make_unique<Graphics>(clientWidth, clientHeight))
{
	// window aspect ratio
	float aspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);	

	// instance models
	std::vector<glm::mat4> instanceModel;
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.5f, 2.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.5f, -2.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 1.5f, 0.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 5.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 1.5f, -0.5f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -1.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.5f, -4.0f)));
	instanceModel.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 1.5f, 3.0f)));
	
	// skybox textures
	std::vector<std::string> skyTextures;
	skyTextures.push_back("../assets/skyboxes/skybox5/px.png");
	skyTextures.push_back("../assets/skyboxes/skybox5/nx.png");
	skyTextures.push_back("../assets/skyboxes/skybox5/py.png");
	skyTextures.push_back("../assets/skyboxes/skybox5/ny.png");
	skyTextures.push_back("../assets/skyboxes/skybox5/pz.png");
	skyTextures.push_back("../assets/skyboxes/skybox5/nz.png");
	
	std::vector<std::string> skyTextures2;
	skyTextures2.push_back("../assets/skyboxes/skybox4/px.png");
	skyTextures2.push_back("../assets/skyboxes/skybox4/nx.png");
	skyTextures2.push_back("../assets/skyboxes/skybox4/py.png");
	skyTextures2.push_back("../assets/skyboxes/skybox4/ny.png");
	skyTextures2.push_back("../assets/skyboxes/skybox4/pz.png");
	skyTextures2.push_back("../assets/skyboxes/skybox4/nz.png");

	// scene objects
	std::vector<std::shared_ptr<Object>> scene_objects;

	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	float angle;
	glm::vec3 camRight;
	glm::vec3 camUp;

	// create street light scene
	scenes.push_back(std::make_shared<Scene>("street light", 0));

	camPos = glm::vec3(5.0f, 15.0f, 15.0f);
	camTarget = glm::vec3(0.0f, 4.5f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	angle = glm::dot(glm::vec3(camDir.x, 0.0f, camDir.z), glm::vec3(0.0f, 0.0f, -1.0f));
	camRight = glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), acos(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(aspectRatio, camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f );
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addPointLight(glm::vec3(-1.5f, 15.0f, -2.5f), glm::vec3(0.025f), glm::vec3(5.0f, 4.5f, 3.0f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);

	scenes[scenes.size()-1]->addObject("../assets/character/ground.assbin", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/ball.assbin", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, -4.0f)));
	scenes[scenes.size()-1]->addObject("../assets/character/ball2.assbin", glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 4.0f, -2.0f)));
	scenes[scenes.size()-1]->addObject("../assets/character/bench.assbin", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/bench.assbin", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/street_light.assbin", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/street_light_bulb.assbin", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/stairs.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/flag_bearer.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/metal.glb", glm::mat4(1.0f));
	scenes[scenes.size()-1]->addObject("../assets/character/flag.glb", glm::mat4(1.0f));

	scenes[scenes.size()-1]->setIBL("../assets/HDRIs/forest.hdr", true);
	scenes[scenes.size()-1]->setGridAxis(8);

	// set physics properties for scene
	worldPhysics.push_back(std::make_unique<WorldPhysics>());
	scene_objects = scenes[scenes.size()-1]->getObjects();
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[0], glm::mat4(1.0f), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[1], glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, -4.0f)), btScalar(0.35), btScalar(0.7), COLLISION_SHAPE::SPHERE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[2], glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 4.0f, -2.0f)), btScalar(0.25), btScalar(0.7), COLLISION_SHAPE::SPHERE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[3], glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 1.5f, 0.0f)), 3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)), btScalar(3.0), btScalar(0.025), COLLISION_SHAPE::BOX);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[4], glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 1.5f, 0.0f)), -3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)), btScalar(3.0), btScalar(0.025), COLLISION_SHAPE::BOX);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[5], glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 4.5f, -2.5f)), btScalar(0.0), btScalar(0.025), COLLISION_SHAPE::CYLINDER);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[6], glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 4.5f, -2.5f)), btScalar(0.0), btScalar(0.025), COLLISION_SHAPE::CYLINDER);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[7], glm::mat4(1.0f), btScalar(0.0), btScalar(0.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[8], glm::mat4(1.0f), btScalar(0.0), btScalar(0.0), COLLISION_SHAPE::TRIANGLE);
	worldPhysics[worldPhysics.size() - 1]->addSoftBody(scene_objects[10], btScalar(0.25));
	worldPhysics[worldPhysics.size() - 1]->setSoftBodyVertexMass(0, 60, 0.0f);
	worldPhysics[worldPhysics.size() - 1]->setSoftBodyVertexMass(0, 62, 0.0f);
	
	// create outdoor scene
	scenes.push_back(std::make_shared<Scene>("outdoor", 1));

	camPos = glm::vec3(0.0f, 5.0f, 8.0f);
	camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	angle = glm::dot(glm::vec3(camDir.x, 0.0f, camDir.z), glm::vec3(0.0f, 0.0f, -1.0f));
	camRight = glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), acos(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[scenes.size()-1]->addCamera(aspectRatio, camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f );
	scenes[scenes.size()-1]->setActiveCamera(0);

	scenes[scenes.size()-1]->addDirectionalLight(glm::vec3(-1.5f, 15.0f, -2.5f), glm::vec3(0.025f), glm::vec3(10.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.5f));

	scenes[scenes.size()-1]->addObject("../assets/outdoor_scene/scene.glb", glm::mat4(1.0f));

	scenes[scenes.size()-1]->setGridAxis(8);

	// set physics properties for scene
	worldPhysics.push_back(std::make_unique<WorldPhysics>());
	scene_objects = scenes[scenes.size()-1]->getObjects();
	worldPhysics[worldPhysics.size() - 1]->addRigidBody(scene_objects[0], glm::mat4(1.0f), btScalar(0.0), btScalar(1.0), COLLISION_SHAPE::TRIANGLE);
}

void Game::draw(float& delta, int width, int height, DRAWING_MODE mode, bool debug, bool debugPhysics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// update physics
	if(debugPhysics)
	{
		worldPhysics[activeScene]->stepSimulation(
				scenes[activeScene]->getActiveCamera()->getViewMatrix(),
				scenes[activeScene]->getActiveCamera()->getProjectionMatrix()
				);
		return;
	}
	worldPhysics[activeScene]->stepSimulation();
	std::vector<std::shared_ptr<Object>> scene_objects = scenes[activeScene]->getObjects();
	int indexPhysics{0};
	for(;indexPhysics < worldPhysics[activeScene]->getNumRigidBody(); ++indexPhysics)
	{
		glm::mat4 model = worldPhysics[activeScene]->getObjectOpenGLMatrix(indexPhysics);
		scene_objects[indexPhysics]->setModel(model);
	}
	if(activeScene == 0)
		indexPhysics++;
	for(int i{0}; i < worldPhysics[activeScene]->getNumSoftBody(); ++i, ++indexPhysics)
	{
		worldPhysics[activeScene]->updateSoftBody(i, scene_objects[indexPhysics]);
	}

	// update character's current animation
	if(character && character->sceneID == scenes[activeScene]->getId())
	{
		character->get()->getAnimator()->updateAnimation(delta);
	}

	// get shader
	Shader s = graphics->getPBRShader();

	if(activeScene < scenes.size())
	{
		if(graphics->getShadowQuality() != SHADOW_QUALITY::OFF)
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

			// BIND TO DEFAULT FRAMEBUFFER
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// DRAW FINAL IMAGE QUAD
			graphics->getFinalShader().use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments()[0].id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getPongFBO()->getAttachments()[0].id);
				graphics->getFinalShader().setInt("bloom", 1);
				graphics->getFinalShader().setInt("bloomEffect", 1);
			}
			else
			{
				graphics->getFinalShader().setInt("bloomEffect", 0);
			}
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

			// bind to default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// draw final image quad
			graphics->getFinalShader().use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments()[0].id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getPongFBO()->getAttachments()[0].id);
				graphics->getFinalShader().setInt("bloom", 1);
				graphics->getFinalShader().setInt("bloomEffect", 1);
			}
			else
			{
				graphics->getFinalShader().setInt("bloomEffect", 0);
			}
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
	// window aspect ratio
	float aspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);	

	#pragma omp for
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i]->getActiveCamera()->updateProjectionMatrix(clientWidth, clientHeight);
	}

	graphics->resizeScreen(clientWidth, clientHeight);
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	if(index < scenes.size())
	{
		scenes[index]->getActiveCamera()->updateViewMatrix(inputs, mouse, delta);
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

void Game::setCharacter(std::string filePath, glm::mat4 aModel, std::string aName)
{
	character = std::make_shared<Character>(filePath, aModel, aName);
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i]->setCharacter(character);
		worldPhysics[i]->setKinematicCharacter(character->get());
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

void Game::directionalShadowPass(int index, float delta, DRAWING_MODE mode)
{
	glViewport(0, 0, static_cast<int>(graphics->getShadowQuality()), static_cast<int>(graphics->getShadowQuality()));
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 0);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 0);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 0);
	graphics->getShadowMappingShader().setMatrix("proj", graphics->getOrthoProjection());

	// render directional depth maps
	int sLightsOffset{0};
	for(int i{0}; i < scenes[index]->getDLights().size(); ++i, ++sLightsOffset)
	{
		graphics->getStdDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes[index]->getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index]->getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		graphics->getShadowMappingShader().setMatrix("view", lightView);

		// draw scene
		scenes[index]->draw(graphics->getShadowMappingShader(), graphics, delta, mode);
	}

	for(int i{0}; i < scenes[index]->getSLights().size(); ++i)
	{
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
	glViewport(0, 0, static_cast<int>(graphics->getShadowQuality()), static_cast<int>(graphics->getShadowQuality()));
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 1);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 1);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 1);

	// render omnidirectional depth maps
	std::vector<glm::mat4> omnilightViews;
	for(int i{0}; i < scenes[index]->getPLights().size(); ++i)
	{
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

	// draw scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	s.use();
	s.setInt("pointLightCount", scenes[index]->getPLights().size());
	s.setVec3f("cam.viewPos", scenes[index]->getActiveCamera()->getPosition());
	s.setMatrix("view", scenes[index]->getActiveCamera()->getViewMatrix());
	s.setMatrix("proj", scenes[index]->getActiveCamera()->getProjectionMatrix());
	s.setLighting(scenes[index]->getPLights(), scenes[index]->getDLights(), scenes[index]->getSLights());
	s.setInt("hasSSAO", graphics->ssaoOn() ? 1 : 0);
	glActiveTexture(GL_TEXTURE0 + 13);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(1)->getAttachments()[0].id);
	s.setInt("ssao", 13);
	s.setVec2f("viewport", glm::vec2(width, height));

	// set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes[index]->getPLights().size();
	int nbDLights = scenes[index]->getDLights().size();
	int nbSLights = scenes[index]->getSLights().size();

	int textureOffset{4};

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
		s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics->getOrthoProjection() * lightView);
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
	glViewport(0, 0, width, height);
	std::unique_ptr<Framebuffer> & ping = graphics->getPingFBO();
	std::unique_ptr<Framebuffer> & pong = graphics->getPongFBO();
	Shader & bloom = graphics->getBloomShader();
	bloom.use();
	bloom.setInt("image", 0);

	// blur image
	bool horizontal{true};
	bool firstIteration{true};
	int amount{10};
	for(int i{0}; i < amount; ++i)
	{
		if(horizontal)
		{
			ping->bind();
			glClear(GL_COLOR_BUFFER_BIT);
			bloom.setInt("horizontal", 1);
			glActiveTexture(GL_TEXTURE0);
			if(firstIteration)
			{
				firstIteration = false;
				glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(1)->getAttachments()[0].id);
			}
			else
				glBindTexture(GL_TEXTURE_2D, pong->getAttachments()[0].id);
			graphics->getQuadMesh()->draw(bloom);
		}
		else
		{
			pong->bind();
			glClear(GL_COLOR_BUFFER_BIT);
			bloom.setInt("horizontal", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ping->getAttachments()[0].id);
			graphics->getQuadMesh()->draw(bloom);
		}
		horizontal = !horizontal;
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
	graphics->getAOBlurShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(0)->getAttachments()[0].id); // raw AO
	graphics->getAOBlurShader().setInt("aoInput", 0);
	graphics->getQuadMesh()->draw(graphics->getAOBlurShader());

	// reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}
