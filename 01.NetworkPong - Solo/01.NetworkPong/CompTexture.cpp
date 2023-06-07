#include "CompTexture.h"
#include "GameObject.h"

CompTexture::CompTexture(Texture* texture)
	: texture{ texture }
{

}

void CompTexture::update()
{
	texture->scale = gameObject->transform.scale;

	int x = gameObject->transform.position.x;
	int y = gameObject->transform.position.y;
	texture->draw(x, y);
}

void CompTexture::destroy()
{
}
