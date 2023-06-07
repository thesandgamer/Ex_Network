#pragma once
#include <SDL_stdinc.h>
#include "GameObject.h"

#include <string>

enum class PacketType
{
	INIT_MATCH,
	PLAYER_1_POSITION,
	PLAYER_2_POSITION,
	BALL_POSITION,
	MISC
};


class Packet
{
public:
	Packet() = default;
	Packet(PacketType type);
	Packet(GameObject* gameObject);
	Packet(string message);


	PacketType packetType{ PacketType::MISC };	//Typ de paquet qu'on va créer

	//Donnés du paquet
	Uint16 id{ 0 };
	Vector2<float> position{ 0.0f, 0.0f };	
	Vector2<float> direction{ 0.0f, 0.0f };

	string msg{"DefaultMessage"};



};