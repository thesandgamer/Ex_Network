#include "NetworkAgent.h"
#include <iostream>
NetworkAgent::NetworkAgent()
{
	if (SDLNet_Init() == -1)
	{
		std::cout << "SDLNet Error at initialization: "
			<< SDLNet_GetError() << "\n";
		return;
	}
}

NetworkAgent::~NetworkAgent()
{
	destroy();
}

void NetworkAgent::destroy()
{
	beforeDestroy();
	SDLNet_Quit();
}

/// <summary>
/// Send a Packet on the network by socket
/// </summary>
/// <param name="socket"></param>
/// <param name="packet"></param>
/// <returns></returns>
bool NetworkAgent::sendPacket(TCPsocket socket, Packet* packet)
{

	//Si on à pas ouvert la connection on ne peut pas envoyer
	if (!socket)
	{
		std::cout << "Can't send packet, socket is not opened \n";
		return false;
	}

	int result;
	int len = sizeof(Packet);
	result = SDLNet_TCP_Send(socket, packet, len);//On va envoyer des données qui vont contenir le packet de la taille du packet dans la connection

	if (result < len)
	{
		std::cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << "\n";
		return false;
	}
	return true;
}

/// <summary>
/// Recezive Packet on socket
/// </summary>
/// <param name="socket"></param>
/// <returns></returns>
Packet* NetworkAgent::recvPacket(TCPsocket socket)
{
	if (!socket)
	{
		std::cout << "Can't receive packet, socket is not opened \n";
		return nullptr;
	}

	Packet* packet = new Packet();
	int result;
	int len = sizeof(Packet);

	result = SDLNet_TCP_Recv(socket, packet, len);


	if (result <= 0)
	{
		std::cout << "SDLNet_TCP_Recv: " << SDLNet_GetError() << "\n";
		return nullptr;
	}
	else
	{
		std::cout << packet->msg << std::endl;
	}

	return packet;
}