/**
 * \author Mathias Velo
 * \date 01/01/2021
 */

#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "window.hpp"
#include "game.hpp"
#include "framebuffer.hpp"

void characterMovements(std::unique_ptr<WindowManager> & client, std::unique_ptr<Game> & game, float delta)
{
	std::bitset<16> inputs{client->getUserInputs()};

	if(inputs.test(10) && !(inputs.test(6) || inputs.test(11))) // spacebar & !forward
	{
		game->characterDoActionJump(false, delta);
	}
	else if(inputs.test(10) && (inputs.test(6) || inputs.test(11))) // spacebar & forward
	{
		game->characterDoActionJump(true, delta);
	}
	else if(inputs.test(6)) // up arrow
	{
		if(inputs.test(8))
			game->characterDoActionWalk(Character::DIRECTION::LEFT, delta);
		else if(inputs.test(9))
			game->characterDoActionWalk(Character::DIRECTION::RIGHT, delta);
		else
			game->characterDoActionWalk(Character::DIRECTION::FRONT, delta);
	}
	else if(inputs.test(11)) // r key
	{
		if(inputs.test(8))
			game->characterDoActionRun(Character::DIRECTION::LEFT, delta);
		else if(inputs.test(9))
			game->characterDoActionRun(Character::DIRECTION::RIGHT, delta);
		else
			game->characterDoActionRun(Character::DIRECTION::FRONT, delta);
	}
	else
	{
		game->characterDoActionIdle();
	}
}

void vehicleCallback(std::unique_ptr<WindowManager> & client, std::unique_ptr<Game> & game)
{
	std::bitset<16> inputs{client->getUserInputs()};

	if(inputs.test(6))
	{
		game->vehicleDrive(0, true);
	}
	if(inputs.test(7))
	{
		game->vehicleDrive(0, false);
	}
	if(inputs.test(8))
	{
		game->vehicleSteering(0, VEHICLE_STEERING::RIGHT);
	}
	if(inputs.test(9))
	{
		game->vehicleSteering(0, VEHICLE_STEERING::LEFT);
	}

	// draw wheels
	game->vehicleSetWheelTransform(0);
}

void render(std::unique_ptr<WindowManager> client, std::unique_ptr<Game> game)
{
	game->setActiveScene(0);

	// >>>>>>>>>>>>>>>>>>>> create collection of characters
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
	game->setCharacter("assets/character/matahy.glb", model, "Matahy", game->getActiveScene(), glm::ivec2(client->getWidth(), client->getHeight()));
	// <<<<<<<<<<<<<<<<<<<< create collection of characters

	// delta
	double currentFrame{0.0f};
	double lastFrame{0.0};
	float delta{0.0f};
	
	while(client->isAlive())
	{
		currentFrame = omp_get_wtime();
		delta = static_cast<float>(currentFrame - lastFrame);
		client->checkEvents();
		game->updateSceneActiveCameraView(game->getActiveScene(), client->getUserInputs(), client->getMouseData(), delta);

		if(client->getUserInputs().test(5))
		{
			game->resizeScreen(client->getWidth(), client->getHeight());
		}

		if(game->getCharacterScene() == game->getActiveScene())
			characterMovements(client, game, delta);
		//vehicleCallback(client, game); // temporary

		// draw scene
		game->draw(delta, client->getWidth(), client->getHeight(), DRAWING_MODE::SOLID, true, false);

		client->resetEvents();
		SDL_GL_SwapWindow(client->getWindowPtr());
		lastFrame = currentFrame;
	}

}

int main(int argc, char* argv[])
{
	std::unique_ptr<WindowManager> client{std::make_unique<WindowManager>("Maze-Engine")};
	std::unique_ptr<Game> game{std::make_unique<Game>(client->getWidth(), client->getHeight())};
	render(std::move(client), std::move(game));

	return 0;
}
