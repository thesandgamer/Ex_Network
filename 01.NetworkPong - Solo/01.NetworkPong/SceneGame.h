#pragma once
#include "Scene.h"
#include "PlayerAI.h"
#include "Ball.h"

enum class GameState {
	Running,
	Finished,
	NetworkError
};

class SceneGame : public Scene
{
public:
	SceneGame(SDL_Renderer* renderer, SceneMode mode);
	void load() override;

	void start();
	void handleEvents(SDL_Event& event) override;
	void onUpdate() override;

	void startNewGame();
	bool isGameFinished();
	void handlePossibleFinishedGame();
	void handlePlayersMovement();
	void switchToMainMenu();
	void reloadGame();

private:
	SDL_Renderer* renderer{ nullptr };
	GameState gameState { GameState::Running };
	SceneMode mode{ SceneMode::SinglePlayer };
	Player* player{ nullptr };
	PlayerAI* playerTwo{ nullptr };
	Ball* ball{ nullptr };
	const Uint8 maxScore{ 3 };

	// Network
	void handleConnectionEstablished() override;
	bool gameConnectedAndCanStart{ false };
	// Thread
	SDL_Thread* thread = nullptr;
	SDL_sem* sem = nullptr;
};

