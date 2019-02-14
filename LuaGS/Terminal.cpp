#include "Terminal.h"

int Terminal::xPos = 0;
int Terminal::yPos = 0;

int Terminal::r = 255;
int Terminal::g = 255;
int Terminal::b = 255;

float Terminal::textScale = 3.f;

void pushCFuncToTable(lua_State *L, char const *name, int(*func)(lua_State *))
{
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_settable(L, -3);
}

void Terminal::initTerminal(lua_State *L)
{
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "term");
	lua_pushvalue(L, -1);

	pushCFuncToTable(L, "refresh", Terminal::refresh);
	pushCFuncToTable(L, "getCursorPos", Terminal::getCursorPos);
	pushCFuncToTable(L, "getTextColor", Terminal::getTextColor);
	pushCFuncToTable(L, "getTextScale", Terminal::getTextScale);
	pushCFuncToTable(L, "setCursorPos", Terminal::setCursorPos);
	pushCFuncToTable(L, "setTextColor", Terminal::setTextColor);
	pushCFuncToTable(L, "setTextScale", Terminal::setTextScale);
	pushCFuncToTable(L, "write", Terminal::write);
}

int Terminal::refresh(lua_State *L)
{
	display->sync();
	lua_pushboolean(L, 1);
	return 1;
}

int Terminal::getCursorPos(lua_State *L)
{
	lua_pushinteger(L, xPos);
	lua_pushinteger(L, yPos);
	return 2;
}

int Terminal::getTextColor(lua_State *L)
{
	lua_pushinteger(L, Terminal::r);
	lua_pushinteger(L, Terminal::g);
	lua_pushinteger(L, Terminal::b);
	return 3;
}

int Terminal::getTextScale(lua_State *L)
{
	lua_pushnumber(L, Terminal::textScale);
	return 1;
}

int Terminal::setCursorPos(lua_State *L)
{
	if (lua_gettop(L) >= 2) {
		xPos = LuaValueHelper::optInt(L, 1, xPos);
		yPos = LuaValueHelper::optInt(L, 2, yPos);
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 2 arguments, got %d\n", lua_gettop(L));
	}
	return 0;
}

int Terminal::setTextColor(lua_State *L)
{
	if (lua_gettop(L) >= 3) {
		r = LuaValueHelper::optInt(L, 1, r);
		g = LuaValueHelper::optInt(L, 2, g);
		b = LuaValueHelper::optInt(L, 3, b);
		
		r = MathHelper::clamp<int>(r, 0, 255);
		g = MathHelper::clamp<int>(g, 0, 255);
		b = MathHelper::clamp<int>(b, 0, 255);
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 3 arguments, got %d\n", lua_gettop(L));
	}
	return 1;
}

int Terminal::setTextScale(lua_State *L)
{
	if (lua_gettop(L) >= 1)
	{
		textScale = LuaValueHelper::optDouble(L, 1, textScale);
		textScale = MathHelper::clamp<float>(textScale, 0.25f, 10.0f);
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 1 arguments, got 0\n");
	}
	return 1;
}

int Terminal::write(lua_State *L)
{
	if (lua_gettop(L) > 0) {
		if (lua_isstring(L, 1)) {
			std::string str(lua_tostring(L, 1));
			glColor3f(r / 255.0, g / 255.0, b / 255.0);
			fontRenderer->drawString(xPos, yPos, str.c_str(), textScale);
		} else {
			lua_pushboolean(L, 0);
			luaL_error(L, "Argument #1 expected LuaString");
			return 1;
		}
	}
	lua_pushboolean(L, 1);
	return 1;
}