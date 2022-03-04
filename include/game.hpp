#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include <thread>
#include <map>
#include <iterator>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"
#include "character.hpp"

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		void draw(float& delta, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false, bool debugPhysics = false);
		void resizeScreen(int clientWidth, int clientHeight);
		void updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		std::unique_ptr<Graphics> & getGraphics();
		std::vector<std::shared_ptr<Scene>> & getScenes();
		int getActiveScene();
		void setActiveScene(int index);
		int getActiveVehicle();
		void setActiveVehicle(int index);
		void setCharacterScene(int index);
		int getCharacterScene();
		void setCharacter(std::string filePath, glm::mat4 aModel, std::string aName, int index, glm::ivec2 scrDim);
		void removeCharacter();
		void characterDoActionWalk(Character::DIRECTION d, float delta);
		void characterDoActionRun(Character::DIRECTION d, float delta);
		void characterDoActionJump(bool forward, float delta);
		void characterDoActionIdle();
		void vehicleDrive(bool forward);
		void vehicleDriveReset();
		void vehicleSteering(VEHICLE_STEERING dir);
		void vehicleSteeringReset();
		void vehicleSetWheelTransform();
		void vehicleUpdateUpVector();

	private:

		int activeScene;
		int activeVehicle;
		std::vector<std::shared_ptr<Scene>> scenes;
		std::vector<std::unique_ptr<WorldPhysics>> worldPhysics;
		std::map<std::string, std::shared_ptr<Object>> loadedAssets;
		std::unique_ptr<Graphics> graphics;
		std::shared_ptr<Character> character;

	private: // render passes

		void directionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void omnidirectionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void bloomPass(int width, int height);
		void ssaoPass(int index, int width, int height, float delta);
		void colorMultisamplePass(int index, int width, int height, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
		void volumetricsPass(int index, int width, int height, float delta);
};

#endif
