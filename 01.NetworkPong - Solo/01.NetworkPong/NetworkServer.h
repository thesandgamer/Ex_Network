#pragma once
#include "NetworkAgent.h"

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
	TCPsocket serverSocket{ nullptr };
	Uint16 serverPort{ 0 };

	// Client side
	TCPsocket clientSocket{ nullptr };
	bool readConfigFile() override;
	bool establishConnection() override;
	bool sendPacket(Packet* packet) override;
	Packet* recvPacket() override;
	void beforeDestroy() override;
	bool openServerSocket();
	bool pairWithClient();
};