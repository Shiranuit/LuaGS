#include "LuaValueHelper.h"

int LuaValueHelper::optInt(lua_State *L, int n, int def)
{
	if (L == nullptr)
		return def;
	if (lua_gettop(L) < 1) {
		luaL_error(L, "Argument %d, LuaNumber expected");
		return def;
	}
	if (lua_isnumber(L, n))
		return (int)lua_tonumber(L, n);
	else
		luaL_error(L, "Argument %d, LuaNumber expected");
	return def;
}

double LuaValueHelper::optDouble(lua_State *L, int n, double def)
{
	if (L == nullptr)
		return def;
	if (lua_gettop(L) < 1) {
		luaL_error(L, "Argument %d, LuaNumber expected");
		return def;
	}
	if (lua_isnumber(L, n))
		return lua_tonumber(L, n);
	else
		luaL_error(L, "Argument %d, LuaNumber expected");
	return def;
}

void LuaValueHelper::pushCFuncToTable(lua_State *L, char const *name, int(*func)(lua_State *))
{
	if (L == nullptr)
		return;
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_settable(L, -3);
}
