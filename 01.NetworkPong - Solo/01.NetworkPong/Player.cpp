#include "Player.h"
#include "Scene.h"

Player::Player(Texture* texture)
{
	collider = new CompCollider(texture);
	setComponent(collider);
	renderer = new CompTexture(texture);
	setComponent(renderer);
}

void Player::onStart()
{
	collider = getComponent<CompCollider>();
	renderer = getComponent<CompTexture>();
}

void Player::move(MoveDirection direction)
{
	if (direction == MoveDirection::MoveUp)
	{
		if (transform.position.y > 0)
			transform.position.y -= speed;
	}
	else
	{
		if (transform.position.y < WINDOW_HEIGHT - collider->getDimensions().y)
			transform.position.y += speed;
	}
}

void Player::addPoint(int point)
{
	score += point;
}

void Player::resetScore()
{
	score = 0;
}
