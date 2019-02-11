#include "LuaBinding.h"

int GlobalCallLua(lua_State *L)
{
	rttr::method *m = (rttr::method *)lua_touserdata(L, lua_upvalueindex(1)); // Retrieve UserData
	rttr::method method(*m); // Avoid Lua Garbage Collector
	rttr::array_range<rttr::parameter_info> methodParams = method.get_parameter_infos();
	int luaArgCount = lua_gettop(L);

	if (methodParams.size() > luaArgCount) {
		luaL_error(L, "Expected %d arguments, got %d\n", methodParams.size(), luaArgCount);
		return 0;
	}
	if (methodParams.size() == 0)
	{
		rttr::variant result = method.invoke({});
		if (!result.is_valid()) {
			luaL_error(L, "Something went wrong\n");
			return 0;
		}
		return 0;
	}

	std::vector<rttr::argument> methodArgs(methodParams.size());
	LuaValue *values = (LuaValue *)malloc(sizeof(LuaValue) * methodParams.size());


	auto paramIterator = methodParams.begin();

	for (int i = 0; i < luaArgCount; i++) {
		int id = i + 1;
		const rttr::type param = paramIterator->get_type();
		int luaType = lua_type(L, id);
	
		switch (luaType)
		{

		case LUA_TNUMBER:
			if (param == rttr::type::get<unsigned long>()) {
				values[i].ulongVal = (unsigned long)lua_tonumber(L, id);
				methodArgs[i] = values[i].ulongVal;
			} else if (param == rttr::type::get<unsigned int>()) {
				values[i].uintVal = (unsigned int)lua_tonumber(L, id);
				methodArgs[i] = values[i].uintVal;
			} else if (param == rttr::type::get<unsigned short>()) {
				values[i].ushortVal = (unsigned char)lua_tonumber(L, id);
				methodArgs[i] = values[i].ushortVal;
			} else if (param == rttr::type::get<unsigned char>()) {
				values[i].ucharVal = (unsigned char)lua_tonumber(L, id);
				methodArgs[i] = values[i].ucharVal;
			} else if (param == rttr::type::get<long>()) {
				values[i].longVal = (long)lua_tonumber(L, id);
				methodArgs[i] = values[i].longVal;
			} else if (param == rttr::type::get<int>()) {
				values[i].intVal = (int)lua_tonumber(L, id);
				methodArgs[i] = values[i].intVal;
			} else if (param == rttr::type::get<short>()) {
				values[i].shortVal = (short)lua_tonumber(L, id);
				methodArgs[i] = values[i].shortVal;
			} else if (param == rttr::type::get<char>()) {
				values[i].byteVal = (char)lua_tonumber(L, id);
				methodArgs[i] = values[i].byteVal;
			} else if (param == rttr::type::get<float>()) {
				values[i].floatVal = (float)lua_tonumber(L, id);
				methodArgs[i] = values[i].floatVal;
			} else if (param == rttr::type::get<double>()) {
				values[i].doubleVal = (double)lua_tonumber(L, id);
				methodArgs[i] = values[i].doubleVal;
			} else {
				luaL_error(L, "Argument #%d Unknown conversion LuaNumber to %s\n", id, param.get_name().to_string().c_str());
				return 0;
			}
			break;
		//case LUA_TSTRING:
		//	if (param == rttr::type::get<char *>()) {
		//		values[i].strVal = std::string(lua_tostring(L, i));
		//		methodArgs[i] = values[i].strVal.c_str();
		//	} else if (param == rttr::type::get<std::string>()) {
		//		values[i].strVal = std::string(lua_tostring(L, i));
		//		methodArgs[i] = values[i].strVal;
		//	} else {
		//		luaL_error(L, "Argument #%d Unknown conversion LuaString to %s\n", i, param.get_name().to_string().c_str());
		//		return 0;
		//	}
		//	break;
		case LUA_TBOOLEAN:
			if (param == rttr::type::get<bool>()) {
				values[i].boolVal = lua_toboolean(L, id);
				methodArgs[i] = values[i].boolVal;
			} else {
				luaL_error(L, "Argument #%d Unknown conversion LuaBoolean to %s\n", id, param.get_name().to_string().c_str());
				return 0;
			}
		case LUA_TTABLE:
			luaL_error(L, "Argument #%d Unknown conversion LuaTable to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		case LUA_TTHREAD:
			luaL_error(L, "Argument #%d Unknown conversion LuaThread to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		case LUA_TFUNCTION:
			luaL_error(L, "Argument #%d Unknown conversion LuaFunction to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		case LUA_TNIL:
			luaL_error(L, "Argument #%d Unknown conversion LuaNil to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		case LUA_TNONE:
			luaL_error(L, "Argument #%d Unknown conversion LuaNone to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		default:
			luaL_error(L, "Unknown type conversion\n");
			return 0;
		}
	}

	rttr::variant result = method.invoke_variadic({}, methodArgs);
	if (!result.is_valid()) {
		luaL_error(L, "Something went wrong\n");
		return 0;
	}
	return 0;

}

void LuaBinding::registerMethod(lua_State *L, const rttr::method &method)
{
	lua_pushlightuserdata(L, (void *)&method);
	lua_pushcclosure(L, GlobalCallLua, 1);
	lua_setglobal(L, method.get_name().to_string().c_str());
}

void LuaBinding::registerTableMethod(lua_State *L, const rttr::method &method)
{
	lua_pushstring(L, method.get_name().to_string().c_str());
	lua_pushlightuserdata(L, (void *)&method);
	lua_pushcclosure(L, GlobalCallLua, 1);
	lua_settable(L, -3);
}