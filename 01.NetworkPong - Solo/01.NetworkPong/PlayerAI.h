#pragma once
#include "Player.h"

class PlayerAI : public Player
{
public:
	PlayerAI(Texture* texture);
	MoveDirection getNextMoveDirection(Vector2<float> ballPosition);
};

