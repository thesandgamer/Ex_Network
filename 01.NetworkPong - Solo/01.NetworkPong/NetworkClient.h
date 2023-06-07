#pragma once
#include "NetworkAgent.h"
#include <string>


enum class ClientState
{
	Null,
	READ_CONFIG_FILE,
	OPENING_SOCKET,
	SENDING,
	RECEIVING,
	CONNECTION_ESTABLISHED
};

//Classe pour cr�er un client pour le r�seau
class NetworkClient : public NetworkAgent
{
public:
	NetworkClient();

	ClientState state;

	// Client side
	TCPsocket clientSocket{ nullptr };

	// Server side
	std::string serverIP{ "" };
	Uint16 serverPort{ 0 };

	bool readConfigFile() override;
	bool establishConnection() override;
	bool sendPacket(Packet* packet) override;
	Packet* recvPacket() override;
	void beforeDestroy() override;
	bool openClientSocket();
};

//		
// De base le client va lire les donn�es de config
// Si il � r�ussit � les lires il va ouvrir une socket
// Si la socket est ouverte, on va envoyer un premier paquet
// Si le paquet c'est bien envoy� il va se mettre en attente d'un paquet
// Si le paquet � bien �t� re�u la connection va �tre etablie
//