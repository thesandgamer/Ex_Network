#include "SceneMenu.h"
#include "Engine.h"
#include <string>
#include <iostream>
#include "SceneGame.h"

using std::string;

SceneMenu::SceneMenu(SDL_Renderer* renderer)
	: Scene(), renderer { renderer }
{
}

void SceneMenu::load()
{
	std::cout << "Loading media \n";

	// Load background
	Texture* backgroundTexture = new Texture("MenuBackGround.png", renderer);
	GameObject* background = new GameObject();
	CompTexture* compTextureBg = new CompTexture(backgroundTexture);
	background->setComponent(compTextureBg);
	background->transform.position.x = 0;
	background->transform.position.y = 0;

	// Load title
	Texture* titleTexture = new Texture("PongTitle.png", renderer);
	GameObject* title = new GameObject();
	CompTexture* compTextureTitle = new CompTexture(titleTexture);
	background->setComponent(compTextureTitle);
	title->transform.position.x = WINDOW_WIDTH / 2.0f - titleTexture->size.x / 2.0f;
	title->transform.position.y = WINDOW_HEIGHT / 64.0f;


	// Load  newGame button
	Texture* newGameButtonTexture = new Texture("NewGameButton.png", renderer);
	newGameButton = new Button(newGameButtonTexture);
	newGameButton->transform.position.x = WINDOW_WIDTH / 2.0f - newGameButtonTexture->size.x / 2.0f;
	newGameButton->transform.position.y = WINDOW_HEIGHT / 3.0f;
	newGameButton->setOnClickListener(std::bind(&SceneMenu::onClickNewGame, this));

	// Load  exitGame button
	Texture* exitGameButtonTexture = new Texture("ExitButton.png", renderer);
	exitButton = new Button(exitGameButtonTexture);
	exitButton->transform.position.x = WINDOW_WIDTH / 2 - exitGameButtonTexture->size.x / 2;
	exitButton->transform.position.y = WINDOW_HEIGHT / 1.2f;
	exitButton->setOnClickListener(std::bind(&SceneMenu::onClickExit, this));

	// Load TwoPlayers Button
	Texture* twoPlayersButtonTexture = new Texture("TwoPlayers.png", renderer);
	twoPlayersButton = new Button(twoPlayersButtonTexture);
	twoPlayersButton->transform.position.x = WINDOW_WIDTH / 2 - twoPlayersButtonTexture->size.x / 2;
	twoPlayersButton->transform.position.y = WINDOW_HEIGHT / 2;
	twoPlayersButton->setOnClickListener(std::bind(&SceneMenu::onClickTwoPlayers, this));

	// Load Online Button
	Texture* onlineButtonTexture = new Texture("OnlineButton.png", renderer);
	onlineButton = new Button(onlineButtonTexture);
	onlineButton->transform.position.x = WINDOW_WIDTH / 2 - onlineButtonTexture->size.x / 2;
	onlineButton->transform.position.y = WINDOW_HEIGHT / 1.5f;
	onlineButton->setOnClickListener(std::bind(&SceneMenu::onClickOnline, this));

	// Load server Button
	Texture* serverButtonTexture = new Texture("ServerButton.png", renderer);
	serverButton = new Button(serverButtonTexture);
	serverButton->transform.position.x = WINDOW_WIDTH / 2 - serverButtonTexture->size.x / 2;
	serverButton->transform.position.y = WINDOW_HEIGHT / 3;
	serverButton->layer = 1;
	serverButton->isActive = false;
	serverButton->setOnClickListener(std::bind(&SceneMenu::onClickServer, this));

	// Load client Button
	Texture* clientButtonTexture = new Texture("ClientButton.png", renderer);
	clientButton = new Button(clientButtonTexture);
	clientButton->transform.position.x = WINDOW_WIDTH / 2 - clientButtonTexture->size.x / 2;
	clientButton->transform.position.y = WINDOW_HEIGHT / 2;
	clientButton->layer = 1;
	clientButton->isActive = false;
	clientButton->setOnClickListener(std::bind(&SceneMenu::onClickClient, this));

	// Load back button
	Texture* backButtonTexture = new Texture("BackButton.png", renderer);
	backButton = new Button(backButtonTexture);
	backButton->transform.position.x = WINDOW_WIDTH / 2 - backButtonTexture->size.x / 2;
	backButton->transform.position.y = WINDOW_HEIGHT / 1.5f;
	backButton->layer = 1;
	backButton->isActive = false;
	backButton->setOnClickListener(std::bind(&SceneMenu::onClickBack, this));

	// Add them to the list
	buttonList.push_back(newGameButton);
	buttonList.push_back(twoPlayersButton);
	buttonList.push_back(exitButton);
	buttonList.push_back(onlineButton);

	// Layer 2
	buttonList.push_back(serverButton);
	buttonList.push_back(clientButton);
	buttonList.push_back(backButton);
}

void SceneMenu::onUpdate()
{

}

void SceneMenu::activateButtonLayer(Uint8 layer)
{
	for (auto const& button : buttonList)
	{
		if (button->layer == layer)
			button->isActive = true;
		else
			button->isActive = false;
	}
}

void SceneMenu::handleEvents(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		handleClickEvent();
		break;
	default:
		break;
	}
}

void SceneMenu::handleClickEvent()
{
	// Get Mouse coordinates
	int x, y;
	SDL_GetMouseState(&x, &y);

	// Check if any button was pressed
	for (auto const& button : buttonList)
	{
		if (!button->isActive)
			continue;

		if (button->isWithinBoundaries(x, y))
		{
			button->onClick();
			break;
		}
	}
}

void SceneMenu::createGame(SceneMode mode)
{
	SceneGame* scene = new SceneGame(renderer, mode);
	Engine::loadScene(scene);
}

void SceneMenu::onClickNewGame()
{
	createGame(SceneMode::SinglePlayer);
}

void SceneMenu::onClickExit()
{
	Engine::quitGame();
}

void SceneMenu::onClickTwoPlayers()
{
	createGame(SceneMode::LocalMultiplayer);
}

void SceneMenu::onClickOnline()
{
	activateButtonLayer(1);
}

void SceneMenu::onClickServer()
{
	std::cout << "You have chosen server Mode" << "\n";
	createGame(SceneMode::OnlineServer);
}

void SceneMenu::onClickClient()
{
	std::cout << "You have chosen client Mode" << "\n";
	createGame(SceneMode::OnlineClient);
}

void SceneMenu::onClickBack()
{
	activateButtonLayer(0);
}