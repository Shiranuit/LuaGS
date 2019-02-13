#pragma once

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "TextureLoader.h"

class FontRenderer
{
public:
	FontRenderer(const std::string &filename, unsigned int w, unsigned int h);
	virtual ~FontRenderer();
	void drawString(int x, int y, const std::string &str, float size);
	void drawString(int x, int y, const char *str, float size);

private:
	void drawChar(int x, int y, unsigned char c, float size);
	float getX(int x);
	float getY(int y);

	GLuint font;
	unsigned int w;
	unsigned int h;
};

