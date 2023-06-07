#pragma once
#include <SDL.h>
#include "Vector2.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Texture
{
public:
	static vector<Texture*> gTextures;
	static void clean();

	Texture(const string& path, SDL_Renderer* renderer);

	SDL_Texture* texture{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	Vector2<float> size;
	Vector2<float> scale;
	string folder{ "resources/" };

	void draw(int x, int y);
	void close();

	string getPath(const string& localPath);
	Uint8 getAlpha();
	void setAlpha(Uint8 alpha);
};

