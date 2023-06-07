#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject()
{
	// Add to the scene updater
	Engine::currentScene->addGameObject(this);

	// Add reference to self in transform component
	transform.gameObject = this;

	// Set GameObject's id
	id = Scene::lastGameObjectID++;
}

void GameObject::load()
{
}

void GameObject::start()
{
	onStart();
	isInitialized = true;
}

void GameObject::onStart()
{
}

void GameObject::setComponent(Component* component)
{
	component->gameObject = this;
	components.push_back(component);
}

void GameObject::update()
{
	// Update every component
	for (auto& component : components)
		if (component->isEnabled)
			component->update();

	// Hook for gameObject updates
	onUpdate();
}

void GameObject::onUpdate()
{
}

void GameObject::destroy()
{
	for (auto& component : components)
		component->destroy();
}

void GameObject::onColliderEnter(CompCollider* collider)
{
}

void GameObject::beforeMove()
{
}

void GameObject::afterMove()
{
}
