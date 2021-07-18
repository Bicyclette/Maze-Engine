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

void render(std::unique_ptr<WindowManager> client, std::unique_ptr<Game> game)
{

	// >>>>>>>>>>>>>>>>>>>> create collection of characters
	game->setCharacter("../assets/character/matahy.glb", glm::mat4(1.0f), "Matahy");
	game->setActiveScene(0);
	game->setCharacterScene(0);
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

		// draw scene
		game->draw(delta, client->getWidth(), client->getHeight(), DRAWING_MODE::SOLID, false, false);

		client->resetEvents();
		SDL_GL_SwapWindow(client->getWindowPtr());
		lastFrame = currentFrame;
	}

}

int main(int argc, char* argv[])
{
	std::unique_ptr<WindowManager> client{std::make_unique<WindowManager>("SDL Window")};
	std::unique_ptr<Game> game{std::make_unique<Game>(client->getWidth(), client->getHeight())};
	render(std::move(client), std::move(game));

	return 0;
}
