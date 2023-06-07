#pragma once
class GameObject;

class Component
{
public:
	virtual ~Component() {};

	bool isEnabled{ true };
	GameObject* gameObject{ nullptr };

	virtual void update() = 0;
	virtual void destroy() = 0;
};
