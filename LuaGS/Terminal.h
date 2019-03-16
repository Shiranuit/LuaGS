#pragma once
#include "EngineVariables.h"
#include "LuaValueHelper.h"
#include "MathHelper.h"

class Terminal
{
public:
	static void init(lua_State *L);

	static int write(lua_State *L);
	static int setCursorPos(lua_State *L);
	static int getCursorPos(lua_State *L);
	static int refresh(lua_State *L);
	static int setBackgroundColor(lua_State *L);
	static int getBackgroundColor(lua_State *L);
	static int setTextColor(lua_State *L);
	static int getTextColor(lua_State *L);
	static int setTextScale(lua_State *L);
	static int getTextScale(lua_State *L);
	static int getSize(lua_State *L);


	static int xPos;
	static int yPos;

	static int r;
	static int g;
	static int b;

	static int br;
	static int bg;
	static int bb;

	static float textScale;
};

