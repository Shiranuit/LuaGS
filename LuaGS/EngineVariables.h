#pragma once

#include "LuaContext.h"
#include "Display.h"
#include "FontRenderer.h"

class EngineVariables
{
public:
	static LuaContext lua;
	static lua_State *main_thread;
	static unsigned int width;
	static unsigned int height;
	static Display *display;
	static FontRenderer *fontRenderer;
};

