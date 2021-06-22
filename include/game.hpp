#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"
#include "worldPhysics.hpp"

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
		void addMainCharacter(std::string filePath, glm::mat4 aModel = glm::mat4(1.0f));
		std::shared_ptr<AnimatedObject> getMainCharacter();
		void addCharacter(std::string filePath, glm::mat4 aModel = glm::mat4(1.0f));
		void removeCharacter(std::string name);
		void mainCharacterDoActionWalk(CHARACTER_DIRECTION direction, float delta);
		void mainCharacterDoActionRun(CHARACTER_DIRECTION direction, float delta);
		void mainCharacterDoActionJump();
		void mainCharacterDoActionIdle();

	private:

		int activeScene;
		std::vector<std::shared_ptr<Scene>> scenes;
		std::unique_ptr<Graphics> graphics;
		std::unique_ptr<WorldPhysics> worldPhysics;
		std::shared_ptr<AnimatedObject> mainCharacter;
		std::vector<std::shared_ptr<AnimatedObject>> characters;
		
		void directionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void omnidirectionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void bloomPass(int width, int height);
		void ssaoPass(int index, int width, int height, float delta);
		void colorMultisamplePass(int index, int width, int height, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
};


#endif
