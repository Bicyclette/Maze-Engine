#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <bitset>
#include <omp.h>
#include <cmath>
#include <memory>
#include "color.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

struct WindowEvent
{
	SDL_Event e;
	const Uint8* keyboardState; // snapshot of the keyboard state
	Uint32 mouseButtonBitMask;
};

// about the bitset which register user inputs
// 0 => right mouse button
// 1 => middle mouse button
// 2 => left mouse button
// 3 => wheel scroll

class WindowManager
{
	public:

		/**
 		* \brief Creates a SDL window, with an OPENGL context, returns a pointer
 		* to the newly allocated window, or nullptr on failure.
 		*/
		WindowManager(const std::string& title);
		~WindowManager();
		int getWidth();
		int getHeight();
		SDL_Window* getWindowPtr();
		std::array<int, 3> & getMouseData();
		std::bitset<16> & getUserInputs();
		bool isAlive();
		void checkEvents();
		void resetEvents();

	private:

		std::string title;
		int width;
		int height;
		bool alive;

		SDL_Window * window;
		SDL_GLContext glContext;

		struct WindowEvent event;
		std::array<int, 3> mouseData; // 0 = xRel, 1 = yRel, 2 = mouse wheel direction
		std::bitset<16> userInputs;
};

#endif
