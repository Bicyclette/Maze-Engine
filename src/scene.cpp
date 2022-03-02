#include "scene.hpp"

Scene::Scene(std::string pName, int aId) :
	name(pName),
	ID(aId)
{}

int Scene::getId()
{
	return ID;
}

void Scene::addObject(std::string filePath, glm::mat4 aModel, std::string collisionFilePath, const std::vector<glm::mat4> & instanceModel)
{
	std::shared_ptr<Object> obj{std::make_shared<Object>(filePath, aModel)};

	if(instanceModel.size() > 0)
	{
		obj->setInstancing(instanceModel);
	}
	if(!collisionFilePath.empty())
	{
		obj->setCollisionShape(collisionFilePath, aModel);
	}

	objects.push_back(obj);
}

void Scene::addObject(std::shared_ptr<Object> obj)
{
	objects.push_back(obj);
}

void Scene::setCharacter(std::shared_ptr<Character> aCharacter)
{
	character = aCharacter;
}

void Scene::removeCharacter()
{
	character.reset();
}

void Scene::addCamera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, float near, float far)
{
	cameras.push_back(std::make_shared<Camera>(type, scrDim, pos, target, up, fov, near, far));
}

void Scene::addPointLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq)
{
	pLights.push_back(std::make_shared<PointLight>(quality, pos, amb, diff, spec, aKc, aKl, aKq));
	pLights[pLights.size() - 1]->setModelMatrix(glm::mat4(1.0f));
}

void Scene::addDirectionalLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float orthoDim)
{
	dLights.push_back(std::make_shared<DirectionalLight>(quality, pos, amb, diff, spec, dir, orthoDim));
	dLights[dLights.size() - 1]->setModelMatrix(glm::mat4(1.0f));
}

void Scene::addSpotLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle)
{
	sLights.push_back(std::make_shared<SpotLight>(quality, pos, amb, diff, spec, dir, innerAngle, outerAngle));
	sLights[sLights.size() - 1]->setModelMatrix(glm::mat4(1.0f));
}

void Scene::setSkybox(std::vector<std::string> & textures, bool flip)
{
	sky = std::make_unique<Skybox>(textures, flip);
}

void Scene::setIBL(std::string texture, bool flip, int clientWidth, int clientHeight)
{
	ibl = std::make_unique<IBL>(texture, flip, clientWidth, clientHeight);
}

void Scene::addParticlesEmitter(glm::vec3 pos, int emitRate, float maxLifetime, ParticleEmitter::DIRECTION direction_type, float speed, glm::vec3 direction_vector)
{
	particlesEmitter.push_back(std::make_unique<ParticleEmitter>(pos, emitRate, maxLifetime, direction_type, speed, direction_vector));
}

void Scene::addLightning(glm::vec3 from, glm::vec3 to, int step, glm::vec3 color, float intensity, std::vector<float> & arcs, bool dynamic, float refreshInterval)
{
	lightning.push_back(std::make_unique<Lightning>(from, to, step, color, intensity, arcs, dynamic, refreshInterval));
}

void Scene::addVehicle(std::shared_ptr<Vehicle> vehicle)
{
	vehicles.push_back(vehicle);
}

std::vector<std::unique_ptr<Lightning>> & Scene::getLightnings()
{
	return lightning;
}

void Scene::setGridAxis(int gridDim)
{
	gridAxis = std::make_unique<GridAxis>(gridDim);
}

void Scene::setActiveCamera(int index)
{
	activeCamera = cameras[index];
}

void Scene::updateCameraPerspective(glm::ivec2 scrDim)
{
	for(int i{0}; i < cameras.size(); ++i)
	{
		std::shared_ptr<Camera> cam = cameras[i];
		cameras[i]->setProjection(scrDim, cam->getNearPlane(), cam->getFarPlane());
	}
}

std::vector<std::shared_ptr<Camera>> & Scene::getCameras()
{
	return cameras;
}

std::shared_ptr<Camera> & Scene::getActiveCamera()
{
	return activeCamera;
}

std::string & Scene::getName()
{
	return name;
}

void Scene::draw(Shader & shader, std::unique_ptr<Graphics> & graphics, float delta, DRAWING_MODE mode, bool debug)
{
	if(debug)
	{
		if(gridAxis)
		{
			gridAxis->draw(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());
		}

		for(int i{0}; i < pLights.size(); ++i)
		{
			pLights[i]->setViewMatrix(activeCamera->getViewMatrix());
			pLights[i]->setProjMatrix(activeCamera->getProjectionMatrix());
			pLights[i]->draw();
		}
		for(int i{0}; i < dLights.size(); ++i)
		{
			dLights[i]->setViewMatrix(activeCamera->getViewMatrix());
			dLights[i]->setProjMatrix(activeCamera->getProjectionMatrix());
			dLights[i]->drawDebug();
		}
		for(int i{0}; i < sLights.size(); ++i)
		{
			sLights[i]->setViewMatrix(activeCamera->getViewMatrix());
			sLights[i]->setProjMatrix(activeCamera->getProjectionMatrix());
			sLights[i]->draw();
		}
	}

	struct IBL_DATA iblData;
	if(ibl && shader.getType() == SHADER_TYPE::PBR)
	{
		shader.use();
		shader.setInt("IBL", 1);
		iblData = ibl->get_IBL_data();
		ibl->draw(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());
	}
	else if(!ibl && shader.getType() == SHADER_TYPE::PBR)
	{
		shader.use();
		shader.setInt("IBL", 0);
	}

	for(int i{0}; i < objects.size(); ++i)
	{
		if(ibl)
			objects[i]->draw(shader, &iblData, mode);
		else
			objects[i]->draw(shader, nullptr, mode);
	}
	
	if(character && character->sceneID == ID)
	{
		if(ibl)
			character->draw(shader, &iblData, mode);
		else
			character->draw(shader, nullptr, mode);
	}

	for(int i{0}; i < particlesEmitter.size(); ++i)
	{
		particlesEmitter[i]->emit(activeCamera->getPosition(), delta);
		if(debug)
			particlesEmitter[i]->drawEmitter(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());
		particlesEmitter[i]->drawParticles(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), activeCamera->getRight(), activeCamera->getUp());
	}

	for(int i{0}; i < lightning.size(); ++i)
	{
		lightning[i]->draw(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), delta);
	}

	if(sky)
	{
		sky->draw(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());
	}
}

std::vector<std::shared_ptr<PointLight>> & Scene::getPLights()
{
	return pLights;
}

std::vector<std::shared_ptr<DirectionalLight>> & Scene::getDLights()
{
	return dLights;
}

std::vector<std::shared_ptr<SpotLight>> & Scene::getSLights()
{
	return sLights;
}

std::vector<std::shared_ptr<Object>> Scene::getObjects()
{
	return objects;
}

std::shared_ptr<Character> Scene::getCharacter()
{
	return character;
}

std::vector<std::shared_ptr<Vehicle>> & Scene::getVehicles()
{
	return vehicles;
}

void Scene::addAudioFile(std::string file)
{
	audio.load_sound(file);
}

void Scene::addSoundSource(glm::vec3 position, float volume, bool loop)
{
	sound_source.emplace_back(position, volume, loop);
}

void Scene::playSound(int source_index, int audio_index)
{
	sound_source[source_index].play_sound(audio.sounds[audio_index]);
}

void Scene::stopSound(int source_index, int audio_index)
{
	sound_source[source_index].stop_sound();
}
