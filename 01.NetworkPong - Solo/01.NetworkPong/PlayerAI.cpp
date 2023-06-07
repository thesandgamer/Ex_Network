#include "PlayerAI.h"

PlayerAI::PlayerAI(Texture* texture) 
	: Player(texture)
{
}

MoveDirection PlayerAI::getNextMoveDirection(Vector2<float> ballPosition)
{
	if (transform.position.y > ballPosition.y)
		return MoveDirection::MoveUp;
	else
		return MoveDirection::MoveDown;
}
