#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cstdlib>
#include "skybox.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "grid_axis.hpp"
#include "graphics.hpp"
#include "character.hpp"
#include "IBL.hpp"
#include "particle.hpp"
#include "audio.hpp"
#include "lightning.hpp"
#include "worldPhysics.hpp"

enum class DRAW_TYPE
{
    DRAW_OPAQUE,
    DRAW_TRANSPARENT,
    DRAW_BOTH
};

class Scene
{
	public:

		Scene(std::string pName, int aId);
		int getId();
		void addObject(std::string filePath, glm::mat4 aModel = glm::mat4(1.0f), std::string collisionFilePath = "", const std::vector<glm::mat4> & instanceModel = std::vector<glm::mat4>());
		void setCharacter(std::shared_ptr<Character> aCharacter);
		void removeCharacter();
		void addCamera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 target = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float near = 0.1f, float far = 100.0f);
		void addPointLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq);
		void addDirectionalLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float orthoDim = 10.0f);
		void addSpotLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle);
		void setSkybox(std::vector<std::string> & textures, bool flip);
		void setIBL(std::string texture, bool flip, int clientWidth, int clientHeight);
		void addParticlesEmitter(glm::vec3 pos, int emitRate, float maxLifetime, ParticleEmitter::DIRECTION direction_type, float speed, glm::vec3 direction_vector = glm::vec3(0.0f));
		void addLightning(glm::vec3 from, glm::vec3 to, int step, glm::vec3 color, float intensity, std::vector<float> & arcs, bool dynamic, float refreshInterval);
		void addVehicle(std::shared_ptr<Vehicle> vehicle);
		std::vector<std::unique_ptr<Lightning>> & getLightnings();
		void setGridAxis(int gridDim);
		void setActiveCamera(int index);
		void updateCameraPerspective(glm::ivec2 scrDim);
		std::vector<Camera>& getCameras();
		Camera& getActiveCamera();
		std::string & getName();
		void draw(Shader & shader, Graphics& graphics, DRAW_TYPE drawType, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
		std::vector<std::shared_ptr<PointLight>> & getPLights();
		std::vector<std::shared_ptr<DirectionalLight>> & getDLights();
		std::vector<std::shared_ptr<SpotLight>> & getSLights();
		std::vector<std::shared_ptr<Object>>& getObjects();
		std::shared_ptr<Character> getCharacter();
		std::vector<std::shared_ptr<Vehicle>> & getVehicles();

		// sound management
		void addAudioFile(std::string file);
		void addSoundSource(glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f), float inner_angle = 0.0f, float outer_angle = 0.0f, float volume = 1.0f, bool loop = false);
		void playSound(int source_index, int audio_index);
		void stopSound(int source_index, int audio_index);

    public:
        static inline glm::vec3 sortCamPos;
    private:
        static int sortTransparentMesh(const void * a, const void * b);

	private:

		int ID;
		std::string name;

		int activeCamera;
		std::vector<Camera> cameras;

		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::pair<std::shared_ptr<Mesh>, int>> opaqueMesh;
		std::vector<std::pair<std::shared_ptr<Mesh>, int>> transparentMesh;
		std::shared_ptr<Character> character;
		std::vector<std::shared_ptr<Vehicle>> vehicles;
		Audio audio; // collection of audio files
		std::vector<Source> sound_source; // collection of sound emitters

		std::vector<std::shared_ptr<PointLight>> pLights;
		std::vector<std::shared_ptr<DirectionalLight>> dLights;
		std::vector<std::shared_ptr<SpotLight>> sLights;
		
		std::unique_ptr<Skybox> sky;
		std::unique_ptr<IBL> ibl;

		std::vector<std::unique_ptr<ParticleEmitter>> particlesEmitter;
		std::vector<std::unique_ptr<Lightning>> lightning;

		std::unique_ptr<GridAxis> gridAxis;
};

#endif
