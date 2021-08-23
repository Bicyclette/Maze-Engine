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

void Scene::addPointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq)
{
	pLights.push_back(std::make_shared<PointLight>(pos, amb, diff, spec, aKc, aKl, aKq));
	pLights.at(pLights.size() - 1)->setModelMatrix(glm::mat4(1.0f));
}

void Scene::addDirectionalLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir)
{
	dLights.push_back(std::make_shared<DirectionalLight>(pos, amb, diff, spec, dir));
	dLights.at(dLights.size() - 1)->setModelMatrix(glm::mat4(1.0f));
}

void Scene::addSpotLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle)
{
	sLights.push_back(std::make_shared<SpotLight>(pos, amb, diff, spec, dir, innerAngle, outerAngle));
	sLights.at(sLights.size() - 1)->setModelMatrix(glm::mat4(1.0f));
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

void Scene::setGridAxis(int gridDim)
{
	gridAxis = std::make_unique<GridAxis>(8);
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
			pLights.at(i)->setViewMatrix(activeCamera->getViewMatrix());
			pLights.at(i)->setProjMatrix(activeCamera->getProjectionMatrix());
			pLights.at(i)->draw();
		}
		for(int i{0}; i < dLights.size(); ++i)
		{
			dLights.at(i)->setViewMatrix(activeCamera->getViewMatrix());
			dLights.at(i)->setProjMatrix(activeCamera->getProjectionMatrix());
			dLights.at(i)->draw(graphics->getOrthoDimension());
		}
		for(int i{0}; i < sLights.size(); ++i)
		{
			sLights.at(i)->setViewMatrix(activeCamera->getViewMatrix());
			sLights.at(i)->setProjMatrix(activeCamera->getProjectionMatrix());
			sLights.at(i)->draw();
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
		objects[i]->draw(shader, &iblData, mode);
	}
	
	if(character && character->sceneID == ID)
	{
		character->draw(shader, &iblData, mode);
	}

	for(int i{0}; i < particlesEmitter.size(); ++i)
	{
		particlesEmitter[i]->emit(activeCamera->getPosition(), delta);
		if(debug)
			particlesEmitter[i]->drawEmitter(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());
		particlesEmitter[i]->drawParticles(activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), activeCamera->getRight(), activeCamera->getUp());
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
