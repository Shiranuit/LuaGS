#pragma once

#include "Lua/lua.hpp"
#include "LuaValueHelper.h"
#include <vector>
#include <iostream>


class FileSystem
{
public:
	static void init(lua_State *L);

	static int list(lua_State *L);
	static int isDir(lua_State *L);
	static int exists(lua_State *L);
	static int getSize(lua_State *L);
	static int getPath(lua_State *L);
	static int makeDir(lua_State *L);
	static int copy(lua_State *L);
	static int isReadOnly(lua_State *L);
};

