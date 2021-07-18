#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include "skybox.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "grid_axis.hpp"
#include "graphics.hpp"
#include "character.hpp"
#include "IBL.hpp"
#include "particle.hpp"
#include "audio.hpp"

class Scene
{
	public:

		Scene(std::string pName, int aId);
		int getId();
		void addObject(std::string filePath, glm::mat4 aModel = glm::mat4(1.0f), const std::vector<glm::mat4> & instanceModel = std::vector<glm::mat4>());
		void setCharacter(std::shared_ptr<Character> aCharacter);
		void removeCharacter();
		void addCamera(float aspectRatio, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 target = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float near = 0.1f, float far = 100.0f);
		void addPointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq);
		void addDirectionalLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir);
		void addSpotLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle);
		void setSkybox(std::vector<std::string> & textures, bool flip);
		void setIBL(std::string texture, bool flip);
		void addParticlesEmitter(glm::vec3 pos, int emitRate, float maxLifetime);
		void setGridAxis(int gridDim);
		void setActiveCamera(int index);
		void updateCameraPerspective(float aspectRatio);
		std::vector<std::shared_ptr<Camera>> & getCameras();
		std::shared_ptr<Camera> & getActiveCamera();
		std::string & getName();
		void draw(Shader & shader, std::unique_ptr<Graphics> & graphics, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
		std::vector<std::shared_ptr<PointLight>> & getPLights();
		std::vector<std::shared_ptr<DirectionalLight>> & getDLights();
		std::vector<std::shared_ptr<SpotLight>> & getSLights();
		std::vector<std::shared_ptr<Object>> getObjects();
		std::shared_ptr<Character> getCharacter();

	private:

		int ID;
		std::string name;

		std::shared_ptr<Camera> activeCamera;
		std::vector<std::shared_ptr<Camera>> cameras;

		std::vector<std::shared_ptr<Object>> objects;
		std::shared_ptr<Character> character;

		std::vector<std::shared_ptr<PointLight>> pLights;
		std::vector<std::shared_ptr<DirectionalLight>> dLights;
		std::vector<std::shared_ptr<SpotLight>> sLights;
		
		std::unique_ptr<Skybox> sky;
		std::unique_ptr<IBL> ibl;

		std::vector<std::unique_ptr<ParticleEmitter>> particlesEmitter;

		std::unique_ptr<GridAxis> gridAxis;
};

#endif
