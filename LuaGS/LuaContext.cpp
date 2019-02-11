#include "LuaContext.h"

LuaContext::LuaContext()
{
	type = 0;
	lua = luaL_newstate();
	luaL_openlibs(lua);
}

LuaContext::LuaContext(lua_State *lua_state)
{
	type = 1;
	lua = lua_state;
}

LuaContext::~LuaContext()
{
	if (type == 0) {
		lua_gc(lua, LUA_GCCOLLECT, 0);
		lua_close(lua);
	}
}

int LuaContext::doFile(char const *file)
{
	return luaL_dofile(lua, file);
}

int LuaContext::doFile(const std::string &file)
{
	return doFile(file.c_str());
}

int LuaContext::loadFile(char const *file)
{
	return luaL_loadfile(lua, file);
}

lua_State *LuaContext::getState()
{
	return lua;
}

void LuaContext::setInt(const std::string &name, int val)
{
	lua_pushinteger(lua, val);
	lua_setglobal(lua, name.c_str());
}