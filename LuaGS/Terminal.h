#pragma once
#include "Engine.h"
#include "LuaValueHelper.h"
#include "MathHelper.h"

class Terminal
{
public:
	static void initTerminal(lua_State *L);

	static int write(lua_State *L);
	static int setCursorPos(lua_State *L);
	static int getCursorPos(lua_State *L);
	static int refresh(lua_State *L);
	static int setTextColor(lua_State *L);
	static int getTextColor(lua_State *L);
	static int setTextScale(lua_State *L);
	static int getTextScale(lua_State *L);


	static int xPos;
	static int yPos;

	static int r;
	static int g;
	static int b;

	static float textScale;
};

