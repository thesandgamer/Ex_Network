#pragma once
#include "Texture.h"
#include "Component.h"

class CompTexture : public Component
{
public:
	CompTexture(Texture* texture);

	Texture* texture;

	void update() override;
	void destroy() override;
};

