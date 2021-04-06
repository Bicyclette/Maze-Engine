#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		void drawScene(float& delta, int index, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
		void resizeScreen(int clientWidth, int clientHeight);
		void updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
		std::unique_ptr<Graphics> & getGraphics();

	private:

		std::vector<std::shared_ptr<Scene>> scenes;
		std::unique_ptr<Graphics> graphics;
		
		void directionalShadowPass(int index, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void omnidirectionalShadowPass(int index, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void bloomPass(int width, int height);
		void ssaoPass(int index, int width, int height);
		void colorMultisamplePass(int index, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
};


#endif
