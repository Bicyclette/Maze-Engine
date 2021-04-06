#include "game.hpp"
#include <glm/gtx/rotate_vector.hpp>

Game::Game(int clientWidth, int clientHeight) :
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
	skyTextures.push_back("../assets/skyboxes/skybox4/px.png");
	skyTextures.push_back("../assets/skyboxes/skybox4/nx.png");
	skyTextures.push_back("../assets/skyboxes/skybox4/py.png");
	skyTextures.push_back("../assets/skyboxes/skybox4/ny.png");
	skyTextures.push_back("../assets/skyboxes/skybox4/pz.png");
	skyTextures.push_back("../assets/skyboxes/skybox4/nz.png");
	
	// create scene
	scenes.push_back(std::make_shared<Scene>("backpack"));
	scenes.at(scenes.size()-1)->addCamera(aspectRatio, glm::vec3(0.0f, 3.0f, 4.0f), glm::vec3(0.0f), glm::normalize(glm::vec3(0.0f, 4.0f, -3.0f)), 45.0f, 0.1f, 100.0f );
	scenes.at(scenes.size()-1)->setActiveCamera(0);

	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(-1.5f, 6.0f, -10.0f), glm::vec3(0.25f), glm::vec3(10.0f), glm::vec3(1.0f), 1.0f, 0.14f, 0.07f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(1.5f, 6.0f, -10.0f), glm::vec3(0.25f), glm::vec3(10.0f), glm::vec3(1.0f), 1.0f, 0.14f, 0.07f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(-1.5f, 3.0f, -10.0f), glm::vec3(0.25f), glm::vec3(10.0f), glm::vec3(1.0f), 1.0f, 0.14f, 0.07f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(1.5f, 3.0f, -10.0f), glm::vec3(0.025f), glm::vec3(10.0f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);
	
	//scenes.at(scenes.size()-1)->addDirectionalLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.025f), glm::vec3(20.0f), glm::vec3(1.0f), glm::vec3(-0.5f, -2.0f, -0.5f));
	//scenes.at(scenes.size()-1)->addSpotLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.025f), glm::vec3(400.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.3f), 25.0f, 30.0f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(3.0f, 5.0f, -3.0f), glm::vec3(2.5f), glm::vec3(50.0f), glm::vec3(10.0f), 1.0f, 0.045f, 0.0075f);
	//scenes.at(scenes.size()-1)->addDirectionalLight(glm::vec3(1.0f, 12.0f, 4.0f), glm::vec3(0.025f), glm::vec3(10.0f), glm::vec3(1.0f), glm::vec3(-0.5f, -1.0f, -1.25f));
	//scenes.at(scenes.size()-1)->addDirectionalLight(glm::vec3(2.0f, 5.0f, -1.0f), glm::vec3(0.025f), glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(-0.5f, -0.35f, 0.4f));
	
	scenes.at(scenes.size()-1)->addPointLight(glm::vec3(2.0f, 3.5f, 5.0f), glm::vec3(0.025f), glm::vec3(5.0f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(0.0f, 2.5f, 7.0f), glm::vec3(0.025f), glm::vec3(5.0f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(0.0f, 7.5f, 0.0f), glm::vec3(0.025f), glm::vec3(5.0f), glm::vec3(1.0f), 1.0f, 0.045f, 0.0075f);
	
	//scenes.at(scenes.size()-1)->addDirectionalLight(glm::vec3(-3.0f, 5.0f, -3.0f), glm::vec3(0.025f), glm::vec3(10.0f), glm::vec3(1.0f), glm::vec3(0.25f, -1.0f, 1.0f));
	//scenes.at(scenes.size()-1)->addDirectionalLight(glm::vec3(-3.0f, 5.0f, -3.0f), glm::vec3(0.025f), glm::vec3(10.0f), glm::vec3(1.0f), glm::vec3(-0.25f, -1.0f, 1.0f));
	//scenes.at(scenes.size()-1)->addSpotLight(glm::vec3(3.0f, 5.0f, -3.0f), glm::vec3(0.025f), glm::vec3(100.0f), glm::vec3(1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), 15.0f, 17.0f);
	
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(2.0f, 3.5f, -7.0f), glm::vec3(0.025f), glm::vec3(25.0f), glm::vec3(1.0f), PointLight::attenuation[6][1], PointLight::attenuation[6][2], PointLight::attenuation[6][3]);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(0.0f, 3.5f, 10.0f), glm::vec3(0.025f), glm::vec3(25.0f), glm::vec3(1.0f), PointLight::attenuation[6][1], PointLight::attenuation[6][2], PointLight::attenuation[6][3]);
	//scenes.at(scenes.size()-1)->addPointLight(glm::vec3(0.0f, 7.5f, 0.0f), glm::vec3(0.025f), glm::vec3(25.0f), glm::vec3(1.0f), PointLight::attenuation[6][1], PointLight::attenuation[6][2], PointLight::attenuation[6][3]);
	
	//scenes.at(scenes.size()-1)->addObject("../assets/flowers/flowers.obj", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/street_light/street_light.obj", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/lacoons/untitled.obj", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/podracer/podracer.gltf", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/owl/owl.glb", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/key/key.glb", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/pbr/pbr.glb", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("/home/bicyclette/M1_IMA/IG3D/second_depth_shadow_mapping/assets/composition/composition.glb", glm::mat4(1.0f));
	scenes.at(scenes.size()-1)->addObject("../assets/shield/shield.glb", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/flowers/scene.glb", glm::mat4(1.0f));
	//scenes.at(scenes.size()-1)->addObject("../assets/walk/walk.glb", glm::mat4(1.0f));
	
	//scenes.at(scenes.size()-1)->setSkybox(skyTextures, false);
	scenes.at(scenes.size()-1)->setGridAxis(8);
}

void Game::drawScene(float& delta, int index, int width, int height, DRAWING_MODE mode, bool debug)
{
	/*
	glm::mat4 r = glm::rotate(delta, glm::vec3(0.0f, 1.0f, 0.0f));
	scenes.at(index)->getSLights().at(0)->setDirection(glm::vec3(r * glm::vec4(scenes.at(index)->getSLights().at(0)->getDirection(), 1.0f)));
	*/
	// get shader
	//Shader s = graphics->getBlinnPhongShader();
	Shader s = graphics->getPBRShader();

	if(index < scenes.size())
	{
		if(graphics->getShadowQuality() != SHADOW_QUALITY::OFF)
		{
			s.use();
			s.setInt("shadowOn", 1);

			// SHADOW PASS : directional & spot light sources
			directionalShadowPass(index, mode);
			
			// SHADOW PASS : point light sources
			omnidirectionalShadowPass(index, mode);

			// SSAO PASS
			if(graphics->ssaoOn())
				ssaoPass(index, width, height);

			// COLOR PASS : multisampling
			colorMultisamplePass(index, width, height, mode, debug);

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
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments().at(0).id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getPongFBO()->getAttachments().at(0).id);
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
				ssaoPass(index, width, height);

			// render to multisample framebuffer
			glViewport(0, 0, width, height);
			graphics->getMultisampleFBO()->bind();

			// draw scene
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			s.setVec3f("cam.viewPos", scenes.at(index)->getActiveCamera()->getPosition());
			s.setMatrix("view", scenes.at(index)->getActiveCamera()->getViewMatrix());
			s.setMatrix("proj", scenes.at(index)->getActiveCamera()->getProjectionMatrix());
			s.setLighting(scenes.at(index)->getPLights(), scenes.at(index)->getDLights(), scenes.at(index)->getSLights());
			scenes.at(index)->draw(s, graphics, mode, debug);
			
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
			glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(0)->getAttachments().at(0).id);
			graphics->getFinalShader().setInt("scene", 0);
			if(graphics->bloomOn())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, graphics->getPongFBO()->getAttachments().at(0).id);
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
		scenes.at(i)->getActiveCamera()->updateProjectionMatrix(clientWidth, clientHeight);
	}

	graphics->resizeScreen(clientWidth, clientHeight);
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	if(index < scenes.size())
	{
		scenes.at(index)->getActiveCamera()->updateViewMatrix(inputs, mouse, delta);
	}
}

std::unique_ptr<Graphics> & Game::getGraphics()
{
	return graphics;
}

void Game::directionalShadowPass(int index, DRAWING_MODE mode)
{
	glViewport(0, 0, static_cast<int>(graphics->getShadowQuality()), static_cast<int>(graphics->getShadowQuality()));
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 0);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 0);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 0);
	graphics->getShadowMappingShader().setMatrix("proj", graphics->getOrthoProjection());

	// render directional depth maps
	int sLightsOffset{0};
	for(int i{0}; i < scenes.at(index)->getDLights().size(); ++i, ++sLightsOffset)
	{
		graphics->getStdDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes.at(index)->getDLights().at(i)->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes.at(index)->getDLights().at(i)->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		graphics->getShadowMappingShader().setMatrix("view", lightView);

		// draw scene
		scenes.at(index)->draw(graphics->getShadowMappingShader(), graphics, mode);
	}

	for(int i{0}; i < scenes.at(index)->getSLights().size(); ++i)
	{
		graphics->getStdDepthFBO(sLightsOffset + i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes.at(index)->getSLights().at(i)->getPosition();
		glm::vec3 lightDirection = scenes.at(index)->getSLights().at(i)->getDirection();
		glm::vec3 lightTarget = lightPosition + lightDirection;
		glm::vec3 up = (lightDirection == glm::vec3(0.0f, -1.0f, 0.0f)) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, up);
		float outerCutOff = scenes.at(index)->getSLights().at(i)->getOuterCutOff();

		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes.at(index)->getActiveCamera()->getNearPlane(),
					scenes.at(index)->getActiveCamera()->getFarPlane()
					);

		graphics->getShadowMappingShader().setMatrix("proj", spotProj);
		graphics->getShadowMappingShader().setMatrix("view", lightView);

		// draw scene
		scenes.at(index)->draw(graphics->getShadowMappingShader(), graphics, mode);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::omnidirectionalShadowPass(int index, DRAWING_MODE mode)
{
	glViewport(0, 0, static_cast<int>(graphics->getShadowQuality()), static_cast<int>(graphics->getShadowQuality()));
	graphics->getShadowMappingShader().use();
	graphics->getShadowMappingShader().setInt("computeWorldPos", 1);
	graphics->getShadowMappingShader().setInt("omniDepthRendering", 1);
	graphics->getShadowMappingShader().setInt("omnilightFragDepth", 1);

	// render omnidirectional depth maps
	std::vector<glm::mat4> omnilightViews;
	for(int i{0}; i < scenes.at(index)->getPLights().size(); ++i)
	{
		graphics->getOmniDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		// (proj * view)
		glm::vec3 lightPosition = scenes.at(index)->getPLights().at(i)->getPosition();
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics->getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		graphics->getShadowMappingShader().setVec3f("lightPosition", lightPosition);
		for(int j{0}; j < 6; ++j)
			graphics->getShadowMappingShader().setMatrix("omnilightViews[" + std::to_string(j) + "]", omnilightViews.at(j));
		omnilightViews.clear();

		// draw scene
		scenes.at(index)->draw(graphics->getShadowMappingShader(), graphics, mode);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::colorMultisamplePass(int index, int width, int height, DRAWING_MODE mode, bool debug)
{
	// render to multisample framebuffer
	glViewport(0, 0, width, height);
	graphics->getMultisampleFBO()->bind();

	// get shader
	//Shader s = graphics->getBlinnPhongShader();
	Shader s = graphics->getPBRShader();

	// draw scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	s.use();
	s.setInt("pointLightCount", scenes.at(index)->getPLights().size());
	s.setVec3f("cam.viewPos", scenes.at(index)->getActiveCamera()->getPosition());
	s.setMatrix("view", scenes.at(index)->getActiveCamera()->getViewMatrix());
	s.setMatrix("proj", scenes.at(index)->getActiveCamera()->getProjectionMatrix());
	s.setLighting(scenes.at(index)->getPLights(), scenes.at(index)->getDLights(), scenes.at(index)->getSLights());
	s.setInt("hasSSAO", graphics->ssaoOn() ? 1 : 0);
	glActiveTexture(GL_TEXTURE0 + 13);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(1)->getAttachments().at(0).id);
	s.setInt("ssao", 13);
	s.setVec2f("viewport", glm::vec2(width, height));

	// set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes.at(index)->getPLights().size();
	int nbDLights = scenes.at(index)->getDLights().size();
	int nbSLights = scenes.at(index)->getSLights().size();

	int textureOffset{4};

	for(int i{0}; i < scenes.at(index)->getPLights().size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_CUBE_MAP, graphics->getOmniDepthFBO(i)->getAttachments().at(0).id);
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i) + "].lightSpaceMatrix", glm::mat4(1.0f));
		textureOffset++;
	}

	int depthMapIndex{0};
	for(int i{0}; i < scenes.at(index)->getDLights().size(); ++i)
	{
		glm::vec3 lightPosition = scenes.at(index)->getDLights().at(i)->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes.at(index)->getDLights().at(i)->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(depthMapIndex)->getAttachments().at(0).id);
		s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics->getOrthoProjection() * lightView);
		depthMapIndex++;
		textureOffset++;
	}

	for(int i{0}; i < scenes.at(index)->getSLights().size(); ++i)
	{
		float outerCutOff = scenes.at(index)->getSLights().at(i)->getOuterCutOff();
		glm::vec3 lightPosition = scenes.at(index)->getSLights().at(i)->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes.at(index)->getSLights().at(i)->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes.at(index)->getActiveCamera()->getNearPlane(),
					scenes.at(index)->getActiveCamera()->getFarPlane()
					);

		glActiveTexture(GL_TEXTURE0 + textureOffset);
		glBindTexture(GL_TEXTURE_2D, graphics->getStdDepthFBO(depthMapIndex)->getAttachments().at(0).id);
		s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		s.setMatrix("light[" + std::to_string(i + nbPLights + nbDLights) + "].lightSpaceMatrix", spotProj * lightView);
		depthMapIndex++;
		textureOffset++;
	}

	scenes.at(index)->draw(s, graphics, mode, debug);
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
				glBindTexture(GL_TEXTURE_2D, graphics->getNormalFBO(1)->getAttachments().at(0).id);
			}
			else
				glBindTexture(GL_TEXTURE_2D, pong->getAttachments().at(0).id);
			graphics->getQuadMesh()->draw(bloom);
		}
		else
		{
			pong->bind();
			glClear(GL_COLOR_BUFFER_BIT);
			bloom.setInt("horizontal", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ping->getAttachments().at(0).id);
			graphics->getQuadMesh()->draw(bloom);
		}
		horizontal = !horizontal;
	}
}

void Game::ssaoPass(int index, int width, int height)
{
	// Fill G BUFFER
	graphics->getGBufferFBO()->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render position, normal, and depth data
	graphics->getGBufferShader().use();
	graphics->getGBufferShader().setMatrix("view", scenes.at(index)->getActiveCamera()->getViewMatrix());
	graphics->getGBufferShader().setMatrix("proj", scenes.at(index)->getActiveCamera()->getProjectionMatrix());
	scenes.at(index)->draw(graphics->getGBufferShader(), graphics);

	// render ambient occlusion data
	graphics->getAOFBO(0)->bind();
	graphics->getAOShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->getGBufferFBO()->getAttachments().at(0).id); // position
	graphics->getAOShader().setInt("positionBuffer", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics->getGBufferFBO()->getAttachments().at(1).id); // normal
	graphics->getAOShader().setInt("normalBuffer", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, graphics->getAONoiseTexture()); // noise texture
	graphics->getAOShader().setInt("noiseTexture", 2);
	std::vector<glm::vec3> aoKernel{graphics->getAOKernel()};
	for(int i{0}; i < 64; ++i)
		graphics->getAOShader().setVec3f("samples[" + std::to_string(i) + "]", aoKernel[i]);
	graphics->getAOShader().setFloat("radius", 1.5f);
	graphics->getAOShader().setFloat("bias", 0.05f);
	graphics->getAOShader().setMatrix("projection", scenes.at(index)->getActiveCamera()->getProjectionMatrix());
	graphics->getAOShader().setFloat("screenWidth", static_cast<float>(width));
	graphics->getAOShader().setFloat("screenHeight", static_cast<float>(height));
	graphics->getQuadMesh()->draw(graphics->getAOShader());

	graphics->getAOFBO(1)->bind();
	graphics->getAOBlurShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->getAOFBO(0)->getAttachments().at(0).id); // raw AO
	graphics->getAOBlurShader().setInt("aoInput", 0);
	graphics->getQuadMesh()->draw(graphics->getAOBlurShader());

	// reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}
