#pragma once
#include "GameObject.h"
#include "CompCollider.h"
#include "CompTexture.h"
#include <functional>

class Button : public GameObject
{
public:
	Button(Texture* texture);

	Uint8 layer = 0;

	const bool isWithinBoundaries(int mousePosX, int mousePosY);
	void onStart() override;

	void setOnClickListener(std::function<void()> callback);
	std::function<void()> onClick;

private:
	CompCollider* collider{ nullptr };
	CompTexture* renderer{ nullptr };
};

