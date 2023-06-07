#pragma once
#include <fstream>
#include <sstream>
#include "Packet.h"
#include <SDL_net.h>

class NetworkAgent
{
public:
	NetworkAgent();
	~NetworkAgent();

	// Configuration
	virtual bool readConfigFile() = 0;
	virtual bool establishConnection() = 0;

	// Communication
	virtual bool sendPacket(Packet* packet) = 0;
	virtual Packet* recvPacket() = 0;

	// Other
	void destroy();
	virtual void beforeDestroy() = 0;

protected:
	// Communication
	bool sendPacket(TCPsocket socket, Packet* packet);
	Packet* recvPacket(TCPsocket socket);

};