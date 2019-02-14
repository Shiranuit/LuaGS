#pragma once
#include "LuaContext.h"
#include "LuaValueHelper.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class OS
{
public:
	static void init(lua_State *L);

	static int timeNano(lua_State *L);
	static int pullEvent(lua_State *L);
};

