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
		if (sendPacket(new Packet()))
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
	return NetworkAgent::sendPacket(clientSocket, packet);
}
Packet* NetworkServer::recvPacket()
{
	return NetworkAgent::recvPacket(clientSocket);
}

void NetworkServer::beforeDestroy()
{
	SDLNet_TCP_Close(serverSocket);
	if (state == ServerState::CONNECTION_ESTABLISHED)
	{
		SDLNet_TCP_Close(clientSocket);
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

	serverSocket = SDLNet_TCP_Open(&ip);	//Ouvre la connection
	if (!serverSocket)
	{
		std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
		return false;
	}
	std::cout << "TCP Read Port " << serverPort << " Opened Successfully! \n";
	return true;
}

bool NetworkServer::pairWithClient()
{
	if (!serverSocket)
	{
		std::cout << "Can't send packet, read socket is not opened \n";
		return false;
	}

	if (clientSocket)
	{
		std::cout << "Client already paired \n";
		return false;
	}

	if (clientSocket = SDLNet_TCP_Accept(serverSocket))	//Set la socket du client avec le premier client qui va se connecter
	{
		std::cout << " Paired succesfully with client \n";
		return true;
	}

	std::cout << " Could not pair with client \n";
	return false;
}