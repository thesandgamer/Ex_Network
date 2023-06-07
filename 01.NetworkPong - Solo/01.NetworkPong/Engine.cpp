#include "Engine.h"
#include <iostream>
#include <SDL_image.h>
#include "SceneMenu.h"

using std::cout;

Scene* Engine::currentScene{ nullptr };
bool Engine::isRunning{ false };

void Engine::run()
{
	// Init
	isRunning = init();
	if (!isRunning)
	{
		cout << "Error on window and renderer init" << "\n";
		close();
		return;
	}

	SceneMenu* startScene = new SceneMenu(renderer);
	loadScene(startScene);

	// Game loop
	while (isRunning)
	{
		// Handle events on queue
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else
				currentScene->handleEvents(e);
		}

		// Update & draw
		SDL_RenderClear(renderer);
		currentScene->update();
		SDL_RenderPresent(renderer);
	}

	currentScene->close();
	delete currentScene;

	close();
}

void Engine::loadScene(Scene* scene)
{
	if (currentScene != nullptr)
	{
		currentScene->close();
		delete currentScene;
	}
	currentScene = scene;
	currentScene->load();
	currentScene->start();
}

void Engine::quitGame()
{
	isRunning = false;
}

bool Engine::init()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	bool startupError{ false };
	window = initWindow();
	startupError = window == nullptr;
	renderer = initRenderer();
	startupError = renderer == nullptr;

	return !startupError;
}

SDL_Window* Engine::initWindow()
{
	return SDL_CreateWindow("Network Pong", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
}

SDL_Renderer* Engine::initRenderer()
{
	return SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Engine::close()
{
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	if (window != nullptr) SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}
