#include "NetworkServer.h"
#include <iostream>
NetworkServer::NetworkServer(): NetworkAgent(), state{ ServerState::READ_CONFIG_FILE }
{
}

/// <summary>
/// Set les données du serveur à partir du fichier de configuration
/// </summary>
/// <returns></returns>
bool NetworkServer::readConfigFile()
{
	// Open File
	std::string configFileName = "resources/server-config.txt";
	std::ifstream configFile(configFileName);
	if (!configFile)
	{
		std::cout << "Couldn't open " << configFileName << "\n";
		return false;
	}
	// Line for reading
	std::string line;
	// Read Server Port
	std::getline(configFile, line);
	serverPort = std::stoi(line);
	std::cout << "Server port " << serverPort << "\n";
	return true;
}

/// <summary>
/// Pour vérfier si la connection entre client et serveur c'est fait
/// </summary>
/// <returns></returns>
bool NetworkServer::establishConnection()
{
	switch (state)
	{
	case ServerState::READ_CONFIG_FILE:
		if (readConfigFile())
			state = ServerState::OPENING_SOCKET;
		break;
	case ServerState::OPENING_SOCKET:
		if (openServerSocket())
			state = ServerState::PAIRING;
		break;
	case ServerState::PAIRING:
		if (pairWithClient())
			state = ServerState::SENDING;
		break;
	case ServerState::SENDING:
		if (sendPacket(new Packet("Server accept your connection")))
			state = ServerState::RECEIVING;
		break;
	case ServerState::RECEIVING:
		if (recvPacket())
			state = ServerState::CONNECTION_ESTABLISHED;
		break;
	case ServerState::CONNECTION_ESTABLISHED:
		return true;
	default:
		break;
	}
	return false;
}

bool NetworkServer::sendPacket(Packet* packet)
{
	for (auto clientSocket : clientSockets)
	{
		return NetworkAgent::sendPacket(clientSocket, packet);
	}
}
Packet* NetworkServer::recvPacket()
{
	if (SDLNet_CheckSockets(clientSet, 0) > 0)//Check si il y a une activité
	{
		for (auto clientSocket : clientSockets)
		{
			if (clientSocket && SDLNet_SocketReady(clientSocket) > 0)
			{
				return NetworkAgent::recvPacket(clientSocket);
			}
		}
	}
}

void NetworkServer::beforeDestroy()
{
	for (auto serverSocket : serverSockets)
	{
		SDLNet_TCP_Close(serverSocket);

	}
	if (state == ServerState::CONNECTION_ESTABLISHED)
	{
		for (auto clientSocket : clientSockets)
		{
			SDLNet_TCP_Close(clientSocket);	//Ferme tous les connections qu'on à ouverrtes
		}
	}
}

bool NetworkServer::openServerSocket()
{
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, NULL, serverPort) == -1)
	{
		std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
		return false;
	}

	TCPsocket serverSocket = SDLNet_TCP_Open(&ip);	//Ouvre la connection

	if (!serverSocket)
	{
		std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
		return false;
	}
	serverSockets.push_back(serverSocket);

	std::cout << "TCP Read Port " << serverPort << " Opened Successfully! \n";
	return true;
}

bool NetworkServer::pairWithClient()
{
	if (!serverSockets.size() > 0)
	{
		std::cout << "Can't send packet, read socket is not opened \n";
		return false;
	}

	/*
	if (clientSocket)
	{
		std::cout << "Client already paired \n";
		return false;
	}
	*/
	TCPsocket tempSocket = SDLNet_TCP_Accept(serverSockets.back());

	if (tempSocket)	//Set la socket du client avec le premier client qui va se connecter
	{
		std::cout << " Paired succesfully with client \n";
		clientSockets.push_back(tempSocket);
		SDLNet_TCP_AddSocket(clientSet, tempSocket);

		//openServerSocket();


		return true;
	}

	std::cout << "\r Could not pair with client \n "<<std::flush;
	return false;
}