#pragma once
#include "stb_image.h"
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

class TextureLoader
{
public:
	static GLuint loadImage(const std::string &filename);
};

