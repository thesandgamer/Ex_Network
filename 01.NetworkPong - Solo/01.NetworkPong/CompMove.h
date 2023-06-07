#pragma once
#include "Component.h"
#include "Vector2.h"

class CompMove : public Component
{
public:
	CompMove(Vector2<float> direction, int speed);

	void update() override;
	void destroy() override;

	Vector2<float> getDirection() ;
	void setDirection(Vector2<float> dir);

	int speed{ 1 };

private:
	Vector2<float> direction{ 0.0f, 0.0f };
};

