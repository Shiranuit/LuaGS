#include "FontRenderer.h"



FontRenderer::FontRenderer(const std::string &_filename, unsigned int _w, unsigned int _h) : w(_w), h(_h)
{
	this->font = TextureLoader::loadImage(_filename);
}


FontRenderer::~FontRenderer()
{
	glDeleteTextures(1, &this->font);
}

void FontRenderer::drawString(int x, int y, const std::string &str, float size)
{
	drawString(x, y, str.c_str(), size);
}

void FontRenderer::drawString(int x, int y, const char *str, float size)
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_RECTANGLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_RECTANGLE, this->font);
	for (int i = 0; str[i]; i++) {
		drawChar(x + i * 6, y, str[i], size);
	}
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glDisable(GL_TEXTURE_RECTANGLE);
	glDisable(GL_BLEND);
}

float FontRenderer::getX(int x)
{
	return (float)x / (float)w * 2.f - 1.f;
}

float FontRenderer::getY(int y)
{
	return (float)(h - y) / (float)h * 2.f - 1.f;
}

void FontRenderer::drawChar(int x, int y, unsigned char c, float size)
{
	static int fontWidth = 6;
	static int fontHeight = 9;
	int i = (c % 16);
	int j = (c / 16);

	int xStart = 1 + i * (fontWidth + 2);
	int yStart = 1 + j * (fontHeight + 2);
	glBegin(GL_QUADS);
	glPushMatrix();
	glTexCoord2f(xStart, yStart);
	glVertex3f(getX(x * size), getY(y * size), 0.f);

	glTexCoord2f((xStart + fontWidth), yStart);
	glVertex3f(getX((x + fontWidth) * size), getY(y * size), 0.f);

	glTexCoord2f((xStart + fontWidth) , (yStart + fontHeight));
	glVertex3f(getX((x + fontWidth) * size), getY((y + fontHeight) * size), 0.f);

	glTexCoord2f(xStart, (yStart + fontHeight));
	glVertex3f(getX(x * size), getY((y + fontHeight) * size), 0.f);
	glPopMatrix();
	glEnd();
}