#include "CompMove.h"
#include "GameObject.h"

CompMove::CompMove(Vector2<float> direction, int speed)
	:speed{speed}, direction{direction}
{
}

void CompMove::update()
{
	gameObject->beforeMove();

	Vector2<float> pos = gameObject->transform.position;
	Vector2<float> displacement = (direction * speed);
	Vector2<float> newPos = pos + displacement;
	gameObject->transform.position = newPos;

	gameObject->afterMove();
}

void CompMove::destroy()
{
}

Vector2<float> CompMove::getDirection()
{
	return direction;
}

void CompMove::setDirection(Vector2<float> dir)
{
	dir.normalize();
	direction = dir;
}
