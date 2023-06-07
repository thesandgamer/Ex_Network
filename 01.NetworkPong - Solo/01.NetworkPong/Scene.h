#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "NetworkAgent.h"
#include <vector>
#include <map>

enum class SceneMode
{
	SinglePlayer,
	LocalMultiplayer,
	OnlineClient,
	OnlineServer
};

constexpr float WINDOW_WIDTH{ 640.0f };
constexpr float WINDOW_HEIGHT{ 480.0f };

int recvPacketThread(void* data);

class Scene
{
public:
	virtual void handleEvents(SDL_Event& e);
	virtual void load();
	virtual void start();
	virtual void update();
	virtual void close();

	virtual void onUpdate();

	// Game object management
	static Uint16 lastGameObjectID;
	std::map<Uint16, GameObject*> gameObjectMap;
	std::vector<GameObject*> gameObjectsToInitialize;

	void addGameObject(GameObject* gameObject);
	void initGameObject(GameObject* gameObject);

	// Network
	NetworkAgent* networkAgent{ nullptr };
	bool connectionEstablished{ false };
	bool disconnected{ false };
	bool alreadyDestroyed{ false };

	SDL_Thread* thread{ nullptr };
	SDL_sem* sem{ nullptr };

	void setSceneMode(SceneMode sceneMode);
	void destroyNetworkAgent();
	void disconnect();
	bool isOnline();
	bool handlePacket(Packet* packet);
	virtual void onDisconnect() {};
	virtual void handleConnectionEstablished() {};

protected:
	SceneMode sceneMode{ SceneMode::SinglePlayer };

	
};

