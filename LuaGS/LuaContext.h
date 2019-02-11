#pragma once
#include "Lua/lua.hpp"
#include <iostream>
#include <iomanip>
class LuaContext
{
public:
	LuaContext();
	LuaContext(lua_State *lua_state);
	virtual ~LuaContext();
	int doFile(char const *file);
	int doFile(const std::string &file);
	int loadFile(char const *file);
	lua_State *getState();
	void setInt(const std::string &name, int val);

private:
	lua_State *lua;
	uint8_t type;
};

