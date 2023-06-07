#pragma once
#include "Component.h"
#include "Texture.h"
#include <vector>

class CompCollider : public Component
{
public:
	// We keep track of each collider
	static std::vector<CompCollider*> gColliders;

	CompCollider(int width, int height);
	CompCollider(Texture* texture);
	virtual ~CompCollider();


	// Collider boundaries
	int top = 0;
	int bottom = 0;
	int right = 0;
	int left = 0;

	// Collider dimensions
	int width = 0;
	int height = 0;

	// Collider offset
	Vector2<float> offset;

	void computeColliderBoundaries();
	Vector2<float> getCollisionCenter();
	bool isCollidingWith(CompCollider* collider);
	void drawCollisionBoundaries(SDL_Renderer* renderer);

	Vector2<float> getDimensions();
	Vector2<float> getOffsetVector();

	void update() override;
	void destroy() override;
};

