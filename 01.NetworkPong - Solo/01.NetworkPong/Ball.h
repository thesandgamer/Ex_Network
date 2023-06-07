#include "GameObject.h"
#include "Player.h"
#include "Vector2.h"
#include "CompMove.h"
#include <vector>
#pragma once

class SceneGame;

enum class WallCollision
{
	None,
	Top,
	Bottom ,
	Left,
	Right
};

class Ball : public GameObject
{
public:
	Ball(Texture* texture, SceneGame* game);

	SceneGame* game{ nullptr };
	Player* player{ nullptr };
	Player* playerTwo{ nullptr };
	bool hasCollidedWithPlayer{ false };
	bool hasCollidedWithPlayerTwo{ false };

	CompTexture* renderer{ nullptr };
	CompMove* move{ nullptr };
	CompCollider* collider{ nullptr };

	WallCollision checkCollisionWithWalls();
	void handlePossibleWallCollision();
	void modifyDirectionFromCollisionWithPlayer(Player* player);
	void onColliderEnter(CompCollider* collider);

	void reset();

	// Motion Blur
	std::vector<Vector2<float>> motionBlurPositions;
	void renderMotionBlur();
	void addMotionBlurPosition(Vector2<float> pos);

	// Hooks
	void onStart() override;
	void onUpdate() override;
	void beforeMove() override;
	void afterMove() override;
};

