#pragma once
#include "LuaContext.h"
class LuaValueHelper
{
public:
	static int optInt(lua_State *L, int n, int def);
	static double optDouble(lua_State *L, int n, double def);
};

