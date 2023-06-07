#pragma once
#include "Scene.h"
#include <SDL.h>

class Engine
{
public:
	void run();
	static Scene* currentScene;
	static bool isRunning;
	static void loadScene(Scene* scene);
	static void quitGame();

private:
	bool init();
	void close();
	SDL_Window* initWindow();
	SDL_Renderer* initRenderer();

	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
};

