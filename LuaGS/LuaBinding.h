#pragma once
#include <rttr/registration>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Lua/lua.hpp"

class LuaBinding
{
public:
	static int luaValueToC(lua_State *L, int id, const rttr::type &param, std::vector<rttr::argument> &args, std::vector<void *> &LuaValue);
	static int CToluaValue(lua_State *L, rttr::variant &value);
	static void registerMethod(lua_State *L, const rttr::method &method);
	static void registerTableMethod(lua_State *L, const rttr::method &method);
};

