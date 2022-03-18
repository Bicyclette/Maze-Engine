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
#include "editorUI.hpp"

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
		game->vehicleDrive(true);
	}
	if(inputs.test(7))
	{
		game->vehicleDrive(false);
	}
	if(inputs.test(8))
	{
		game->vehicleSteering(VEHICLE_STEERING::RIGHT);
	}
	if(inputs.test(9))
	{
		game->vehicleSteering(VEHICLE_STEERING::LEFT);
	}
	if(!inputs.test(6) && !inputs.test(7))
	{
		game->vehicleDriveReset();
	}
	if(!inputs.test(8) && !inputs.test(9))
	{
		game->vehicleSteeringReset();
	}

	// draw wheels
	game->vehicleSetWheelTransform();

	// Up vector
	game->vehicleUpdateUpVector();
}

void editorUI(EDITOR_UI_SETTINGS & settings, std::unique_ptr<WindowManager> & client, std::unique_ptr<Game> & game, bool & debugPhysics, float & delta)
{
    // ImGui frame init code
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(client->getWindowPtr());
    ImGui::NewFrame();

    // >>>>>>>>>> IMGUI
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("shadows");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("ON", &settings.shadows, 1);
    ImGui::RadioButton("OFF", &settings.shadows, 0);
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(150, 0));
    ImGui::Begin("bloom");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("ON", &settings.bloom, 1);
    ImGui::RadioButton("OFF", &settings.bloom, 0);
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(300, 0));
    ImGui::Begin("tone mapping");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("Reinhard", &settings.tone_mapping, 0);
    ImGui::RadioButton("ACES", &settings.tone_mapping, 1);
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(450, 0));
    ImGui::Begin("volumetrics");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("ON", &settings.volumetrics, 1);
    ImGui::RadioButton("OFF", &settings.volumetrics, 0);
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(600, 0));
    ImGui::Begin("motion blur");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("ON", &settings.motion_blur, 1);
    ImGui::RadioButton("OFF", &settings.motion_blur, 0);
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(750, 0));
    ImGui::Begin("show physics");
    ImGui::SetWindowSize(ImVec2(150, 80));
    ImGui::RadioButton("ON", &settings.show_physics, 1);
    ImGui::RadioButton("OFF", &settings.show_physics, 0);
    ImGui::End(); 

    ImGui::SetNextWindowPos(ImVec2(0, 80));
    ImGui::Begin("AO");
    ImGui::SetWindowSize(ImVec2(210, 160));
    ImGui::RadioButton("ON", &settings.AO, 1);
    ImGui::RadioButton("OFF", &settings.AO, 0);
    ImGui::InputFloat("radius", &settings.AORadius, 0.05f);
    ImGui::InputInt("samples", &settings.AOSamples, 1);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(client->getWidth()-50, 0));
    ImGui::Begin("FPS");
    ImGui::SetWindowSize(ImVec2(50, 60));
    int fps = static_cast<int>(1.0f/delta);
    ImGui::Text(std::to_string(fps).c_str());
    ImGui::End();

    // <<<<<<<<<< IMGUI

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // change engine states
    if(settings.shadows == 1)
        game->getGraphics()->setShadows(true);
    else
        game->getGraphics()->setShadows(false);
    if(settings.bloom == 1)
        game->getGraphics()->setBloomEffect(true);
    else
        game->getGraphics()->setBloomEffect(false);
    if(settings.motion_blur == 1)
        game->getGraphics()->motionBlurFX = true;
    else
        game->getGraphics()->motionBlurFX = false;
    if(settings.AO == 1)
    {
        game->getGraphics()->setSSAOEffect(true);
        game->getGraphics()->setAORadius(settings.AORadius);
        game->getGraphics()->setAOSampleCount(settings.AOSamples);
    }
    else
        game->getGraphics()->setSSAOEffect(false);
    if(settings.tone_mapping == 0)
        game->getGraphics()->set_tone_mapping(TONE_MAPPING::REINHARD);
    else
        game->getGraphics()->set_tone_mapping(TONE_MAPPING::ACES);
    if(settings.volumetrics == 0)
        game->getGraphics()->setVolumetricLighting(false);
    else
        game->getGraphics()->setVolumetricLighting(true);

    if(settings.show_physics == 1)
        debugPhysics = true;
    else
        debugPhysics = false;
}

void render(std::unique_ptr<WindowManager> client, std::unique_ptr<Game> game)
{
    // IMGUI data
    EDITOR_UI_SETTINGS editor_settings;
    
    // start
    DRAWING_MODE draw_mode{DRAWING_MODE::SOLID};
    bool debug{true};
    bool debugPhysics{false};
	game->setActiveScene(0);
	//game->setActiveVehicle(0);

	// >>>>>>>>>>>>>>>>>>>> create collection of characters
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
	//game->setCharacter("assets/character/matahy.glb", model, "Matahy", game->getActiveScene(), glm::ivec2(client->getWidth(), client->getHeight()));
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
		if(game->getActiveVehicle() != -1)
			vehicleCallback(client, game);

		// draw scene
		game->draw(delta, currentFrame, client->getWidth(), client->getHeight(), draw_mode, debug, debugPhysics);

        // draw editor UI
        if(debug)
            editorUI(editor_settings, client, game, debugPhysics, delta);

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
