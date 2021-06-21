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

void render(std::unique_ptr<WindowManager> client, std::unique_ptr<Game> game)
{
	//float brightnessThreshold{10.0f};

	// delta
	double currentFrame{0.0f};
	double lastFrame{0.0};
	float delta{0.0f};
	
	while(client->isAlive())
	{
		client->checkEvents();
		currentFrame = omp_get_wtime();
		delta = static_cast<float>(currentFrame - lastFrame);
		// >>>>>>>>>> IMGUI
		/*
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(client->getWindowPtr());
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Brightness");
		ImGui::SetWindowSize(ImVec2(400, 70));
		ImGui::InputFloat("bloom threshold", &brightnessThreshold, 0.1f);
		ImGui::End();
		*/
		// >>>>>>>>>> IMGUI
		game->updateSceneActiveCameraView(0, client->getUserInputs(), client->getMouseData(), delta);

		if(client->getUserInputs().test(5))
		{
			game->resizeScreen(client->getWidth(), client->getHeight());
		}
		if(client->getUserInputs().test(6))
		{
			if(client->getUserInputs().test(8))
				game->mainCharacterDoActionWalk(CHARACTER_DIRECTION::LEFT);
			else if(client->getUserInputs().test(9))
				game->mainCharacterDoActionWalk(CHARACTER_DIRECTION::RIGHT);
			else
				game->mainCharacterDoActionWalk(CHARACTER_DIRECTION::FRONT);
		}
		else if(client->getUserInputs().test(10))
		{
			game->mainCharacterDoActionJump();
		}
		else if(client->getUserInputs().test(11))
		{
			if(client->getUserInputs().test(8))
				game->mainCharacterDoActionRun(CHARACTER_DIRECTION::LEFT);
			else if(client->getUserInputs().test(9))
				game->mainCharacterDoActionRun(CHARACTER_DIRECTION::RIGHT);
			else
				game->mainCharacterDoActionRun(CHARACTER_DIRECTION::FRONT);
		}
		else
		{
			game->mainCharacterDoActionIdle();
		}
		
		// draw scene
		game->draw(delta, client->getWidth(), client->getHeight(), DRAWING_MODE::SOLID, true, false);

		client->resetEvents();
		// >>>>>>>>>> IMGUI
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// >>>>>>>>>> IMGUI
		SDL_GL_SwapWindow(client->getWindowPtr());
		lastFrame = currentFrame;

		// SEND DATA FROM IMGUI TO GAMES DATA
		/*
		game->getGraphics()->getPBRShader().use();
		game->getGraphics()->getBlinnPhongShader().use();
		game->getGraphics()->getPBRShader().setFloat("brightnessThreshold", brightnessThreshold);
		game->getGraphics()->getBlinnPhongShader().setFloat("brightnessThreshold", brightnessThreshold);
		*/
	}

}

int main(int argc, char* argv[])
{
	std::unique_ptr<WindowManager> client{std::make_unique<WindowManager>("SDL Window")};
	std::unique_ptr<Game> game{std::make_unique<Game>(client->getWidth(), client->getHeight())};
	game->addMainCharacter("../assets/character/my_character.glb", glm::mat4(1.0f));
	render(std::move(client), std::move(game));

	return 0;
}
