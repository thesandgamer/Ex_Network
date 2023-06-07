#include "CompCollider.h"
#include "GameObject.h"

std::vector<CompCollider*> CompCollider::gColliders{};

CompCollider::CompCollider(Texture* texture) 
	: CompCollider(texture->size.x, texture->size.y)
{
}

CompCollider::CompCollider(int widthP, int heightP) 
	: Component(), width{ widthP }, height{ heightP }
{
	// Add to static list
	gColliders.push_back(this);
}

CompCollider::~CompCollider()
{
}

void CompCollider::computeColliderBoundaries()
{
	Vector2<float> position = gameObject->transform.position;
	Vector2<float> dimensions = getDimensions();
	Vector2<float> offsetVector = getOffsetVector();

	int fWidth = dimensions.x;
	int fHeight = dimensions.y;
	int xOffset = offsetVector.x;
	int yOffset = offsetVector.y;

	left = position.x + xOffset;
	right = position.x + fWidth + xOffset;
	top = position.y + yOffset;
	bottom = position.y + fHeight + yOffset;
}

Vector2<float> CompCollider::getCollisionCenter()
{
	int x = (float)(left + right) / 2;
	int y = (float)(top + bottom) / 2;

	return Vector2<float>(x, y);
}

void CompCollider::update()
{
	computeColliderBoundaries();

	for (auto const& collider : gColliders)
	{
		if (this->gameObject == collider->gameObject)
			continue;
		else if (isCollidingWith(collider))
			gameObject->onColliderEnter(collider);
	}
}

bool CompCollider::isCollidingWith(CompCollider* collider)
{
	if (right <= collider->left)
		return false;

	if (left >= collider->right)
		return false;

	if (bottom <= collider->top)
		return false;

	if (top >= collider->bottom)
		return false;

	return true;
}

void CompCollider::drawCollisionBoundaries(SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = left;
	rect.w = right - left;
	rect.y = bottom;
	rect.h = top - bottom;
	SDL_RenderDrawRect(renderer, &rect);
}

void CompCollider::destroy()
{
	gColliders.erase(
		std::remove(gColliders.begin(), gColliders.end(), this), 
		gColliders.end()
	);
}

Vector2<float> CompCollider::getDimensions()
{
	int fWidth = width * gameObject->transform.scale.x;
	int fHeight = height * gameObject->transform.scale.y;

	return Vector2<float>(fWidth, fHeight);
}

Vector2<float> CompCollider::getOffsetVector()
{
	int xOffset = offset.x * gameObject->transform.scale.x;
	int yOffset = offset.y * gameObject->transform.scale.y;

	return Vector2<float>(xOffset, yOffset);
}