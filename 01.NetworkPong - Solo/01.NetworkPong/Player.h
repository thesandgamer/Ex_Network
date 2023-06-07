#pragma once
#include "CompCollider.h"
#include "GameObject.h"
#include "CompTexture.h"

enum class MoveDirection
{
	MoveUp,
	MoveDown
};

class Player : public GameObject
{
public:
	Player(Texture* texture);

	CompCollider* collider{ nullptr };
	CompTexture* renderer{ nullptr };

	void onStart() override;

	void move(MoveDirection direction);
	void addPoint(int point = 1);
	void resetScore();
	int getScore() { return score; }

private:
	int score{ 0 };
	int speed{ 5 };
};

