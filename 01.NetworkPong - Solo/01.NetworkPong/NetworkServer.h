#pragma once
#include "NetworkAgent.h"

#include <vector>

enum class ServerState
{
	Null,
	READ_CONFIG_FILE,
	OPENING_SOCKET,
	PAIRING,
	SENDING,
	RECEIVING,
	CONNECTION_ESTABLISHED
};

class NetworkServer : public NetworkAgent
{
public:
	NetworkServer();

	ServerState state;

	// Server side
	std::vector<TCPsocket> serverSockets;
	Uint16 serverPort{ 0 };

	// Client side
	std::vector<TCPsocket> clientSockets;
	SDLNet_SocketSet clientSet = SDLNet_AllocSocketSet(10);

	bool readConfigFile() override;
	bool establishConnection() override;
	bool sendPacket(Packet* packet) override;
	Packet* recvPacket() override;
	void beforeDestroy() override;
	bool openServerSocket();
	bool pairWithClient();
};