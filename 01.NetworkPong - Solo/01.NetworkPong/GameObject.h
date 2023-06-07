#pragma once
#include "Texture.h"
#include "Vector2.h"
#include "CompTransform.h"
#include "CompCollider.h"
#include <string>
#include <vector>
#include <string>

class GameObject
{
public:
	GameObject();

	std::string name{ "" };
	Uint16 id{ 0 };
	bool isActive{ true };
	CompTransform transform {};

	virtual void load();

	virtual void start();
	virtual void onStart();

	virtual void update();
	virtual void onUpdate();

	void setComponent(Component* component);

	template <typename T>
	T* getComponent()
	{
		for (auto component : components)
		{
			if (T* subComponent = dynamic_cast<T*>(component))
			{
				return subComponent;
			}
		}

		return nullptr;
	}

	virtual void destroy();

	// Hooks
	virtual void onColliderEnter(CompCollider* collider);
	virtual void beforeMove();
	virtual void afterMove();

	// Network
	bool updateFromClient{ false };

private:
	std::vector<Component*> components;
	bool isInitialized = false;



};

