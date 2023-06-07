#pragma once
#include "Component.h"
#include "Vector2.h"

class CompTransform : public Component
{
public:
	Vector2<float> position { 0.0f, 0.0f };
	Vector2<float> scale{ 1.0f, 1.0f };
	double rotation{ 0.0 };

	void update() override {};
	void destroy() override {};
};

