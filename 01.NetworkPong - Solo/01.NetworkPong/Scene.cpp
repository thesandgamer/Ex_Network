#include "Scene.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "CompMove.h"

Uint16 Scene::lastGameObjectID{ 0 };

void Scene::handleEvents(SDL_Event& e)
{
}

void Scene::load()
{
}

void Scene::start()
{
}

void Scene::update()
{
	// Init gameObjects
	while (!gameObjectsToInitialize.empty())
	{
		initGameObject(gameObjectsToInitialize.front());
	}

	//Si le jeu est online, on va tenter de se connecter
	if (isOnline())
	{
		// Try to establish connection
		if (!connectionEstablished)
		{
			connectionEstablished = networkAgent->establishConnection();
			if (connectionEstablished)
				handleConnectionEstablished();
			return;
		}
	}

	// Scene update hook
	onUpdate();

	// Update every object
	for (auto& gameObjectPair : gameObjectMap)
	{
		GameObject* gameObject = gameObjectPair.second;
		if (gameObjectPair.second->isActive)
		{
			gameObject->update();
		}

		//Si le jeu est en ligne
		if (isOnline())
		{
			if (sceneMode == SceneMode::OnlineServer)	//Si on est le serveur
			{
				//On va créer un paquet avec les données de nos objets
				Packet* packet = new Packet(gameObject);
				networkAgent->sendPacket(packet);
			}
			else if (sceneMode == SceneMode::OnlineClient)	//Si on est le client
			{
				if (gameObject->updateFromClient)	//Si l'objet doit se mettre à jour du client
				{
					//On va créer un paquet avec les données de nos objets
					Packet* packet = new Packet(gameObject);
					networkAgent->sendPacket(packet);
				}
			}
		}
	}
}

void Scene::close()
{
	for (auto& gameObject : gameObjectsToInitialize)
	{
		gameObject->destroy();
		delete gameObject;
	}

	gameObjectsToInitialize.clear();

	for (auto& gameObjectPair : gameObjectMap)
	{
		gameObjectPair.second->destroy();
		delete gameObjectPair.second;
	}

	gameObjectMap.clear();

	destroyNetworkAgent();

	Texture::clean();
}

void Scene::onUpdate()
{
}

void Scene::addGameObject(GameObject* gameObject)
{
	gameObjectsToInitialize.push_back(gameObject);
}

void Scene::initGameObject(GameObject* gameObject)
{
	gameObject->start();
	gameObjectsToInitialize.erase(gameObjectsToInitialize.begin());
	gameObjectMap.insert_or_assign(gameObject->id, gameObject);
}

void Scene::setSceneMode(SceneMode sceneModeP)
{
	sceneMode = sceneModeP;
	switch (sceneMode)
	{
	case SceneMode::OnlineClient:
		networkAgent = new NetworkClient();
		break;
	case SceneMode::OnlineServer:
		networkAgent = new NetworkServer();
		break;
	}

	if (isOnline())
	{
		// Threading
		sem = SDL_CreateSemaphore(1);
		thread = SDL_CreateThread(recvPacketThread, "defu", this);
		SDL_DetachThread(thread);
	}
}

void Scene::destroyNetworkAgent()
{
	if (alreadyDestroyed)
		return;
	if (isOnline())
	{
		if (sem)
		{
			SDL_DestroySemaphore(sem);
			sem = nullptr;
		}
		networkAgent->destroy();
		alreadyDestroyed = true;
	}
}

void Scene::disconnect()
{
	onDisconnect();
	disconnected = true;
}

bool Scene::isOnline()
{
	return sceneMode == SceneMode::OnlineClient
		|| sceneMode == SceneMode::OnlineServer;
}

/// <summary>
/// Gère la récéption de paquets
/// </summary>
/// <param name="packet"></param>
/// <returns></returns>
bool Scene::handlePacket(Packet* packet)
{
	if (!packet)
		return false;

	//Set l'objet duquel on reçoit des données 
	Uint16 id = packet->id;
	GameObject* gameObject = gameObjectMap.at(id);

	if (sceneMode == SceneMode::OnlineServer)	//Si on est le serveur
	{
		if (gameObject->updateFromClient)	//Si l'objet doit être mis à jour 
			gameObject->transform.position = packet->position;	//Change sa position
	}
	else if (sceneMode == SceneMode::OnlineClient)	//Si on est le client
	{
		if (!gameObject->updateFromClient)	//Si l'objet doit être mis à jour 
			gameObject->transform.position = packet->position;

		CompMove* move = gameObject->getComponent<CompMove>();
		if (move)
			move->setDirection(packet->direction);
	}
	return true;
}

int recvPacketThread(void* data)
{
	Scene* scene = (Scene*)data;
	Packet* packet = nullptr;
	bool valid = false;

	while (true)
	{
		if (scene->disconnected)
			break;
		if (!scene->connectionEstablished)
			continue;

		// Receive data
		packet = scene->networkAgent->recvPacket();
		SDL_SemWait(scene->sem);
		if (!packet)
		{
			scene->disconnect();
			SDL_SemPost(scene->sem);
			break;
		}
		valid = scene->handlePacket(packet);
		if (!valid)
		{
			scene->disconnect();
			SDL_SemPost(scene->sem);
			break;
		}
		SDL_SemPost(scene->sem);
	}
	return 0;
}