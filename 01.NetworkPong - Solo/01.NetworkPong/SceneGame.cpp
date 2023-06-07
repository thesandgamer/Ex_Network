#include "SceneGame.h"
#include "SceneMenu.h"
#include "Engine.h"
SceneGame::SceneGame(SDL_Renderer* renderer, SceneMode mode): Scene(), renderer{ renderer }
{
	setSceneMode(mode);
}

void SceneGame::load()
{
	// Load background
	Texture* backgroundTexture = new Texture("PongBackGround.png", renderer);
	GameObject* background = new GameObject();
	CompTexture* bgTextureComponent = new CompTexture(backgroundTexture);
	background->setComponent(bgTextureComponent);

	// Load player sprite
	Texture* playerTexture = new Texture("player.png", renderer);

	// Load ball sprite
	Texture* ballTexture = new Texture("PongBallYellow.png", renderer);

	// Create GameObjects
	player = new Player(playerTexture);
	playerTwo = new PlayerAI(playerTexture);
	ball = new Ball(ballTexture, this);

	// Set up Ball
	ball->player = player;
	ball->playerTwo = playerTwo;

	// Networking
	playerTwo->updateFromClient = true;	//Le joueur deux(paddle) doit être mis à jour par le client
}

void SceneGame::start()
{
	startNewGame();
}

void SceneGame::handleEvents(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			disconnect();
			switchToMainMenu();
			break;
		case SDLK_e:
			if (gameState == GameState::Finished)
				switchToMainMenu();
			break;
		case SDLK_r:
			if (gameState == GameState::Finished)
				reloadGame();
		default:
			break;
		}
	}
}

void SceneGame::onUpdate()
{
	// Check if match has endend
	if (!isGameFinished() && gameConnectedAndCanStart)	//Si le jeu est pas finit et si on peut lancer le jeu(2 connecté si online)
	{
		// Start - Critical Section
		if (isOnline())
			SDL_SemWait(sem);

		if (disconnected)
			switchToMainMenu();

		// Handle movement
		handlePlayersMovement();

		// End - Critical Section
		if (disconnected)
			SDL_SemPost(sem);
	}
	else
	{
		ball->move->isEnabled = false;
	}
}

void SceneGame::startNewGame()
{
	// Set player positions
	player->transform.position.x = 20;
	player->transform.position.y = WINDOW_HEIGHT / 2 - player->collider->height / 2;
	playerTwo->transform.position.x = WINDOW_WIDTH - 20 - playerTwo->collider->width;
	playerTwo->transform.position.y = WINDOW_HEIGHT / 2 - playerTwo->collider->height / 2;

	// ResetBall
	ball->reset();

	// Wait for network
	if (isOnline())
	{
		if (connectionEstablished)
		{
			gameConnectedAndCanStart = true;
		}
	}
	else gameConnectedAndCanStart = true;
}

bool SceneGame::isGameFinished()
{
	handlePossibleFinishedGame();
	return gameState == GameState::Finished;
}

void SceneGame::handlePossibleFinishedGame()
{
	if (player->getScore() < maxScore && playerTwo->getScore() < maxScore)
		return;

	gameState = GameState::Finished;

	// Killing networkAgent for avoiding issues when replay is pressed
	gameConnectedAndCanStart = false;
	destroyNetworkAgent();
}

void SceneGame::handlePlayersMovement()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	// Player One movement
	if (sceneMode != SceneMode::OnlineClient)	//Si tu est le serveur tu joue player1
	{
		if (currentKeyStates[SDL_SCANCODE_W])
			player->move(MoveDirection::MoveUp);
		else if (currentKeyStates[SDL_SCANCODE_S])
			player->move(MoveDirection::MoveDown);
	}
	else	//Si tu est le client tu joue le player deux
	{
		// If Online
		if (currentKeyStates[SDL_SCANCODE_W])
			playerTwo->move(MoveDirection::MoveUp);
		else if (currentKeyStates[SDL_SCANCODE_S])
			playerTwo->move(MoveDirection::MoveDown);
	}

	switch (sceneMode)
	{
		// Player Two movement
	case SceneMode::LocalMultiplayer:
		if (currentKeyStates[SDL_SCANCODE_UP])
			playerTwo->move(MoveDirection::MoveUp);
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
			playerTwo->move(MoveDirection::MoveDown);
		break;

	case SceneMode::OnlineClient:
		break;

	case SceneMode::OnlineServer:
		break;

	default:
		Vector2<float> ballPosition(ball->transform.position.x, ball->transform.position.y);
		MoveDirection nextMove = playerTwo->getNextMoveDirection(ballPosition);
		playerTwo->move(nextMove);

		break;
	}
}

void SceneGame::switchToMainMenu()
{
	gameConnectedAndCanStart = false;
	SceneMenu* menu = new SceneMenu(renderer);
	Engine::loadScene(menu);
}

void SceneGame::reloadGame()
{
	SceneGame* game = new SceneGame(renderer, sceneMode);
	Engine::loadScene(game);
}

void SceneGame::handleConnectionEstablished()
{
	gameConnectedAndCanStart = true;
}