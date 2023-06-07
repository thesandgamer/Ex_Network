#include "Packet.h"
#include "CompMove.h"


Packet::Packet(PacketType type): packetType{ type }
{

}


Packet::Packet(GameObject* gameObject): packetType{ PacketType::MISC },id{ gameObject->id },position{ gameObject->transform.position }
{
	auto move = gameObject->getComponent<CompMove>();
	if (move)
	{
		direction = gameObject->getComponent<CompMove>()->getDirection();
	}
}

Packet::Packet(string message): msg{ message }
{
}
