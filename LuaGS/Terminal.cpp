#include "Terminal.h"

int Terminal::xPos = 0;
int Terminal::yPos = 0;

int Terminal::r = 255;
int Terminal::g = 255;
int Terminal::b = 255;

int Terminal::br = 0;
int Terminal::bg = 0;
int Terminal::bb = 0;

float Terminal::textScale = 3.f;

void Terminal::init(lua_State *L)
{
	if (L == nullptr) {
		std::cout << "Error Loading TerminalAPI" << std::endl;
		exit(84);
	}
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "term");
	lua_pushvalue(L, -1);

	LuaValueHelper::pushCFuncToTable(L, "refresh", Terminal::refresh);
	LuaValueHelper::pushCFuncToTable(L, "getSize", Terminal::getSize);
	LuaValueHelper::pushCFuncToTable(L, "getCursorPos", Terminal::getCursorPos);
	LuaValueHelper::pushCFuncToTable(L, "getBackgroundColor", Terminal::getBackgroundColor);
	LuaValueHelper::pushCFuncToTable(L, "setBackgroundColor", Terminal::setBackgroundColor);
	LuaValueHelper::pushCFuncToTable(L, "getTextColor", Terminal::getTextColor);
	LuaValueHelper::pushCFuncToTable(L, "setTextColor", Terminal::setTextColor);
	LuaValueHelper::pushCFuncToTable(L, "getTextScale", Terminal::getTextScale);
	LuaValueHelper::pushCFuncToTable(L, "setCursorPos", Terminal::setCursorPos);
	LuaValueHelper::pushCFuncToTable(L, "setTextScale", Terminal::setTextScale);
	LuaValueHelper::pushCFuncToTable(L, "write", Terminal::write);
}

int Terminal::refresh(lua_State *L)
{
	if (L == nullptr)
		return 0;
	EngineVariables::display->sync();
	lua_pushboolean(L, 1);
	return 1;
}

int Terminal::getCursorPos(lua_State *L)
{
	if (L == nullptr)
		return 0;
	lua_pushinteger(L, xPos);
	lua_pushinteger(L, yPos);
	return 2;
}

int Terminal::getTextColor(lua_State *L)
{
	if (L == nullptr)
		return 0;
	lua_pushinteger(L, Terminal::r);
	lua_pushinteger(L, Terminal::g);
	lua_pushinteger(L, Terminal::b);
	return 3;
}


int Terminal::getBackgroundColor(lua_State *L)
{
	if (L == nullptr)
		return 0;
	lua_pushinteger(L, Terminal::br);
	lua_pushinteger(L, Terminal::bg);
	lua_pushinteger(L, Terminal::bb);
	return 3;
}

int Terminal::getTextScale(lua_State *L)
{
	if (L == nullptr)
		return 0;
	lua_pushnumber(L, Terminal::textScale);
	return 1;
}

int Terminal::setCursorPos(lua_State *L)
{
	if (L == nullptr)
		return 0;
	int ac = lua_gettop(L);
	if (ac >= 2) {
		xPos = LuaValueHelper::optInt(L, 1, xPos);
		yPos = LuaValueHelper::optInt(L, 2, yPos);
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 2 arguments, got %d\n", ac);
	}
	return 0;
}

int Terminal::setTextColor(lua_State *L)
{
	if (L == nullptr)
		return 0;
	int ac = lua_gettop(L);
	if (ac >= 3) {
		r = LuaValueHelper::optInt(L, 1, r);
		g = LuaValueHelper::optInt(L, 2, g);
		b = LuaValueHelper::optInt(L, 3, b);
		
		r = MathHelper::clamp<int>(r, 0, 255);
		g = MathHelper::clamp<int>(g, 0, 255);
		b = MathHelper::clamp<int>(b, 0, 255);
		EngineVariables::fontRenderer->setTextColor(r, g, b);
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 3 arguments, got %d\n", ac);
	}
	return 1;
}

int Terminal::setBackgroundColor(lua_State *L)
{
	if (L == nullptr)
		return 0;
	int ac = lua_gettop(L);
	if (ac >= 3) {
		br = LuaValueHelper::optInt(L, 1, br);
		bg = LuaValueHelper::optInt(L, 2, bg);
		bb = LuaValueHelper::optInt(L, 3, bb);

		br = MathHelper::clamp<int>(br, 0, 255);
		bg = MathHelper::clamp<int>(bg, 0, 255);
		bb = MathHelper::clamp<int>(bb, 0, 255);
		EngineVariables::fontRenderer->setBackgroundColor(br, bg, bb);
		lua_pushboolean(L, 1);
	}
	else {
		lua_pushboolean(L, 0);
		luaL_error(L, "Expected 3 arguments, got %d\n", ac);
	}
	return 1;
}

int Terminal::setTextScale(lua_State *L)
{
	if (L == nullptr)
		return 0;
	int ac = lua_gettop(L);
	if (ac >= 1)
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

int Terminal::getSize(lua_State *L)
{
	if (L == nullptr)
		return 0;
	lua_pushinteger(L, EngineVariables::width);
	lua_pushinteger(L, EngineVariables::height);
	return 2;
}

int Terminal::write(lua_State *L)
{
	if (L == nullptr)
		return 0;
	int ac = lua_gettop(L);
	if (ac > 0) {
		if (lua_isstring(L, 1)) {
			std::string str(lua_tostring(L, 1));
			glColor3f(r / 255.0, g / 255.0, b / 255.0);
			EngineVariables::fontRenderer->drawString(xPos, yPos, str.c_str(), textScale);
		} else {
			lua_pushboolean(L, 0);
			luaL_error(L, "Argument #1 expected LuaString");
			return 1;
		}
	}
	lua_pushboolean(L, 1);
	return 1;
}