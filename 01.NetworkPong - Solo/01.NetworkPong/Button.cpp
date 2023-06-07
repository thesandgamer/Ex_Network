#include "Button.h"

Button::Button(Texture* texture) : GameObject()
{
	setComponent(new CompTexture(texture));
	setComponent(new CompCollider(texture));
}

const bool Button::isWithinBoundaries(int mousePosX, int mousePosY)
{
	int x = mousePosX;
	int y = mousePosY;

	if (x < collider->left)
		return false;
	else if (x > collider->right)
		return false;
	else if (y < collider->top)
		return false;
	else if (y > collider->bottom)
		return false;
	else
		return true;
}

void Button::onStart()
{
	collider = getComponent<CompCollider>();
	renderer = getComponent<CompTexture>();
}

void Button::setOnClickListener(std::function<void()> callback)
{
	onClick = callback;
}
