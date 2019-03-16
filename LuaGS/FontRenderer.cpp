#include "FontRenderer.h"



FontRenderer::FontRenderer(const std::string &_filename, unsigned int _w, unsigned int _h) : w(_w), h(_h)
{
	this->font = TextureLoader::loadImage(_filename);
	fr = 255;
	fg = 255;
	fb = 255;

	br = 0;
	bg = 0;
	bb = 0;
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

	glDisable(GL_TEXTURE_RECTANGLE);
	glDisable(GL_BLEND);

	glBegin(GL_QUADS); // BEGIN DRAW BACKGROUND

	glColor3f(br / 256.0, bg / 256.0, bb / 256.0);
	glVertex3f(getX(x * size), getY(y * size), 0.5f);

	glColor3f(br / 256.0, bg / 256.0, bb / 256.0);
	glVertex3f(getX((x + fontWidth) * size), getY(y * size), 0.5f);

	glColor3f(br / 256.0, bg / 256.0, bb / 256.0);
	glVertex3f(getX((x + fontWidth) * size), getY((y + fontHeight) * size), 0.5f);

	glColor3f(br / 256.0, bg / 256.0, bb / 256.0);
	glVertex3f(getX(x * size), getY((y + fontHeight) * size), 0.5f);

	glEnd(); // END DRAW BACKGROUND


	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_RECTANGLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glBegin(GL_QUADS); // BEGIN DRAW TEXT
	glColor3f(fr / 256.f, fg / 256.f, fb / 256.f);

	glTexCoord2f(xStart, yStart);
	glVertex3f(getX(x * size), getY(y * size), 0.1f);

	glColor3f(fr / 256.f, fg / 256.f, fb / 256.f);
	glTexCoord2f((xStart + fontWidth), yStart);
	glVertex3f(getX((x + fontWidth) * size), getY(y * size), 0.1f);

	glColor3f(fr / 256.f, fg / 256.f, fb / 256.f);
	glTexCoord2f((xStart + fontWidth) , (yStart + fontHeight));
	glVertex3f(getX((x + fontWidth) * size), getY((y + fontHeight) * size), 0.1f);

	glColor3f(fr / 256.f, fg / 256.f, fb / 256.f);
	glTexCoord2f(xStart, (yStart + fontHeight));

	glVertex3f(getX(x * size), getY((y + fontHeight) * size), 0.1f);
	glEnd(); // END DRAW TEXT
}

void FontRenderer::setTextColor(int r, int g, int b)
{
	fr = r;
	fg = g;
	fb = b;
}

void FontRenderer::setBackgroundColor(int r, int g, int b)
{
	br = r;
	bg = g;
	bb = b;
}