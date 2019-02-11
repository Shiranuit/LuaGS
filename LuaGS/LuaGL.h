#pragma once

#include "LuaBinding.h"
#include "Display.h"
#include "Engine.h"

#define loadConst(L, var) lua_pushstring(L, #var); \
						  lua_pushinteger(L, var); \
						  lua_settable(L, -3);


#define loadMethod(var) rttr::registration::method(#var, &##var);

class LuaGL {
public:
	static void loadGLMethods(lua_State *L);
	static void loadGLConsts(lua_State *L);
	static void LoadLuaGL(lua_State *L);
};

