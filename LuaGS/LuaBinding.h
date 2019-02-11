#pragma once
#include <rttr/registration.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Lua/lua.hpp"

union LuaValue
{
	char byteVal;
	short shortVal;
	int intVal;
	long longVal;
	float floatVal;
	double doubleVal;
	bool boolVal;
	unsigned long ulongVal;
	unsigned int uintVal;
	unsigned short ushortVal;
	unsigned char ucharVal;

};

class LuaBinding
{
public:
	static void registerMethod(lua_State *L, const rttr::method &method);
	static void registerTableMethod(lua_State *L, const rttr::method &method);
};

