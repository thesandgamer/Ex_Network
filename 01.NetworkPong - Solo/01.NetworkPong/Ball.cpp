#include "Ball.h"
#include "CompTexture.h"
#include "Scene.h"
#include "SceneGame.h"
#include "Player.h"
#include <time.h>

Ball::Ball(Texture* texture, SceneGame* game) 
	: GameObject(), game{game}
{
	renderer = new CompTexture(texture);
	setComponent(renderer);

	collider = new CompCollider(42, 42);
	collider->offset = Vector2<float>(12, 12);
	setComponent(collider);

	move = new CompMove(Vector2<float>(), 6);
	setComponent(move);
}

WallCollision Ball::checkCollisionWithWalls()
{
	WallCollision collision = WallCollision::None;

	if (collider->top <= 0)
		collision = WallCollision::Top;

	if (collider->bottom >= WINDOW_HEIGHT)
		collision = WallCollision::Bottom;

	if (collider->right >= WINDOW_WIDTH)
		collision = WallCollision::Right;

	if (collider->left <= 0)
		collision = WallCollision::Left;

	return collision;
}

void Ball::handlePossibleWallCollision()
{
	WallCollision collision = checkCollisionWithWalls();
	if (collision != WallCollision::None)
	{
		Vector2<float> dir;
		switch (collision)
		{
		case WallCollision::Top:
		case WallCollision::Bottom:
			dir = move->getDirection();
			move->setDirection(Vector2<float>(dir.x, -dir.y));
			break;

		case WallCollision::Left:
			playerTwo->addPoint();
			game->startNewGame();
			break;
		case WallCollision::Right:
			player->addPoint();
			game->startNewGame();
			break;

		default:
			break;
		}
	}
}

void Ball::modifyDirectionFromCollisionWithPlayer(Player* player)
{
	Vector2<float> ballCenter = collider->getCollisionCenter();
	Vector2<float> padCenter = player->collider->getCollisionCenter();

	int ballRelativeYpos = ballCenter.y - padCenter.y;
	float playerMaxValue = (player->collider->height / 2) + collider->height / 2;
	float centerRate = ballRelativeYpos / playerMaxValue;

	float xDir = -move->getDirection().x;
	float yDir = centerRate;

	move->setDirection(Vector2<float>(xDir, yDir));
	move->speed += 1;
}

void Ball::onColliderEnter(CompCollider* collider)
{
	Player* player = dynamic_cast<Player*>(collider->gameObject);
	if (player)
	{
		modifyDirectionFromCollisionWithPlayer(player);
	}
}

void Ball::reset()
{
	//  Reset Speed
	move->isEnabled = false;
	move->speed = 6;

	// Reset Motion Blur
	motionBlurPositions.clear();

	// Set on the center
	transform.position.x = WINDOW_WIDTH / 2 - renderer->texture->size.x / 2;
	transform.position.y = WINDOW_HEIGHT / 2 - renderer->texture->size.y / 2;

	// Randomize direction
	srand(time(NULL));
	int x = rand() % 100 - 50;
	int y = rand() % 100 - 50;
	Vector2<float> direction(x, y);
	direction.normalize();

	if (1 - fabs(direction.y) < 0.2 && fabs(direction.x) < 0.2)
	{
		direction.y = 0.5;
		direction.x = 0.5;
	}
	move->setDirection(direction);
	move->isEnabled = true;
}

void Ball::renderMotionBlur()
{
	int factor = 128 / (motionBlurPositions.size() + 1);
	for (int i = 0; i < motionBlurPositions.size(); i++)
	{
		Vector2<float> pos = motionBlurPositions.at(i);
		renderer->texture->setAlpha(i * factor);
		renderer->texture->draw(pos.x, pos.y);
	}
	renderer->texture->setAlpha(255);
}

void Ball::addMotionBlurPosition(Vector2<float> pos)
{
	motionBlurPositions.push_back(pos);

	// We want just five positions to render 
	if (motionBlurPositions.size() > move->speed) 
	{
		motionBlurPositions.erase(motionBlurPositions.begin());
	}
}

void Ball::onStart()
{
	collider = getComponent<CompCollider>();
	renderer = getComponent<CompTexture>();
}

void Ball::onUpdate()
{
	renderMotionBlur();
}

void Ball::beforeMove()
{
	// Check wall collisions
	handlePossibleWallCollision();
}

void Ball::afterMove()
{
	// Add to motion blur vector
	addMotionBlurPosition(Vector2<float>(transform.position.x, transform.position.y));
}
