#pragma once
#include "Scene.h"
#include "Button.h"

class SceneMenu : public Scene
{
public:
	SceneMenu(SDL_Renderer* renderer);

	void load() override;
	void onUpdate() override;
	void handleEvents(SDL_Event& e) override;

	void activateButtonLayer(Uint8 layer);
	void handleClickEvent();
	void createGame(SceneMode mode);

	// Button methods
	void onClickNewGame();
	void onClickExit();
	void onClickTwoPlayers();
	void onClickOnline();

	// Layer 2 Button Methods
	void onClickServer();
	void onClickClient();
	void onClickBack();

private:
	SDL_Renderer* renderer{ nullptr };

	// Widgets
	Button* newGameButton{ nullptr };
	Button* exitButton{ nullptr };
	Button* twoPlayersButton{ nullptr };
	Button* onlineButton{ nullptr };

	// Layer 2 Widgets
	Button* serverButton{ nullptr };
	Button* clientButton{ nullptr };
	Button* backButton{ nullptr };

	// Widget list
	std::vector<Button*> buttonList;
};

