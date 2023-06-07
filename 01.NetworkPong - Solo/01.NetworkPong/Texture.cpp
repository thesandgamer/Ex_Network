#include "Texture.h"
#include <SDL_image.h>
#include <iostream>

using std::cout;

vector<Texture*> Texture::gTextures {};

void Texture::clean()
{
	for (Texture* t : gTextures)
	{
		delete(t);
	}
	gTextures.clear();
}

Texture::Texture(const string& path, SDL_Renderer* renderer) : renderer { renderer }
{
	const string completePath = getPath(path);
	const char* charPath = completePath.c_str();
	std::cout << "Loading texture" << completePath << "\n";
	SDL_Surface* imgSurface = IMG_Load(completePath.c_str());
	if (!imgSurface)
	{
		cout << "Unable to load png file from " << charPath
			<< "! SDL Error: " << SDL_GetError() << "\n";
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, imgSurface);

		if (!texture)
		{
			cout << "Unable to optimize surface from " << charPath
				<< "! SDL Error: " << SDL_GetError() << "\n";
		}
		else
		{
			size = Vector2<float>((float)imgSurface->w, (float)imgSurface->h);
			scale = { 1, 1 };
		}

		SDL_FreeSurface(imgSurface);
		gTextures.push_back(this);
	}
}

void Texture::draw(int x, int y)
{
	SDL_Rect renderQuad = { x, y, (int)size.x, (int)size.y };
	renderQuad.w = (int)((float)renderQuad.w * scale.x);
	renderQuad.h = (int)((float)renderQuad.h * scale.y);
	SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
}

void Texture::close()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
		size = { 0, 0 };
	}
}

string Texture::getPath(const string& localPath)
{
	return folder + localPath;
}

Uint8 Texture::getAlpha()
{
	Uint8 alpha;
	SDL_GetTextureAlphaMod(texture, &alpha);
	return alpha;
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}
