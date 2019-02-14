#include "LuaBinding.h"

char *strcopy(const char *str)
{
	int len = 0;
	while (str[len++]);

	char *nstr = (char *)malloc(sizeof(char) * (len));

	for (int i = 0; i < len - 1; i++)
		nstr[i] = str[i];
	nstr[len - 1] = '\0';

	return nstr;
}

const char *cstrcopy(const char *str)
{
	int len = 0;
	while (str[len++]);

	char *nstr = (char *)malloc(sizeof(char) * (len));

	for (int i = 0; i < len - 1; i++)
		nstr[i] = str[i];
	nstr[len - 1] = '\0';

	return nstr;
}

int GlobalCallLua(lua_State *L)
{
	rttr::method *m = (rttr::method *)lua_touserdata(L, lua_upvalueindex(1)); // Retrieve UserData
	if (m == NULL) {
		luaL_error(L, "Error calling the function");
		return 0;
	}
	rttr::method method(*m); // Avoid Lua Garbage Collector
	auto& methodParams = method.get_parameter_infos();
	int luaArgCount = lua_gettop(L);
	int paramCount = methodParams.size();

	if (paramCount > luaArgCount) {
		luaL_error(L, "[%s] Expected %d arguments, got %d\n", method.get_name().to_string().c_str(), methodParams.size(), luaArgCount);
		return 0;
	}
	if (paramCount == 0)
	{
		rttr::variant result = method.invoke({});
		if (!result.is_valid()) {
			luaL_error(L, "Something went wrong\n");
			return 0;
		}
		return 0;
	}

	std::vector<rttr::argument> methodArgs(paramCount);
	std::vector<void *> LuaValue(paramCount);
	for (int i = 0; i < paramCount; i++)
		LuaValue[i] = NULL;

	auto paramIterator = methodParams.begin();

	for (int i = 0; i < luaArgCount; i++, paramIterator++) {
		int id = i + 1;
		const rttr::type param = paramIterator->get_type();
		if (LuaBinding::luaValueToC(L, id, param, methodArgs, LuaValue) < 1)
			return 0;
	}

	/*for (int i = 0; i < luaArgCount; i++) {
		printf("%s\n", methodArgs[i].get_type().get_name().to_string().c_str());
		if (methodArgs[i].get_type() == rttr::type::get<LuaString>()) {
			printf("=====\n");
			printf("%s\n", (char const *)methodArgs[i].get_value<LuaString>());
			printf("=====\n");
		}
	}*/

	rttr::variant result = method.invoke_variadic({}, methodArgs);



	if (!result.is_valid()) {
		luaL_error(L, "Something went wrong\n");
		return 0;
	}
	int valuePushed = LuaBinding::CToluaValue(L, result);
	
	for (int i = 0; i < paramCount; i++)
		delete LuaValue[i];

	return valuePushed;
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

int LuaBinding::CToluaValue(lua_State *L, rttr::variant &value)
{
	if (value.get_type() == rttr::type::get<long>())
		lua_pushnumber(L, value.get_value<long>());
	else if (value.get_type() == rttr::type::get<int>())
		lua_pushnumber(L, value.get_value<int>());
	else if (value.get_type() == rttr::type::get<short>())
		lua_pushnumber(L, value.get_value<short>());
	else if (value.get_type() == rttr::type::get<char>())
		lua_pushnumber(L, value.get_value<char>());
	else if (value.get_type() == rttr::type::get<float>())
		lua_pushnumber(L, value.get_value<float>());
	else if (value.get_type() == rttr::type::get<double>())
		lua_pushnumber(L, value.get_value<double>());
	else if (value.get_type() == rttr::type::get<unsigned long>())
		lua_pushnumber(L, value.get_value<unsigned long>());
	else if (value.get_type() == rttr::type::get<unsigned int>())
		lua_pushnumber(L, value.get_value<unsigned int>());
	else if (value.get_type() == rttr::type::get<unsigned short>())
		lua_pushnumber(L, value.get_value<unsigned short>());
	else if (value.get_type() == rttr::type::get<unsigned char>())
		lua_pushnumber(L, value.get_value<unsigned char>());
	else if (value.get_type() == rttr::type::get<bool>())
		lua_pushboolean(L, value.get_value<bool>());
	else if (value.get_type() == rttr::type::get<const char *>())
		lua_pushstring(L, value.get_value<const char *>());
	else if (value.get_type() == rttr::type::get<char *>())
		lua_pushstring(L, strcopy(value.get_value<char *>()));
	else if (value.get_type() == rttr::type::get<std::string>())
		lua_pushstring(L, strcopy(value.get_value<std::string>().c_str()));
	else if (value.get_type() == rttr::type::get<std::string *>())
		lua_pushstring(L, strcopy(((std::string *)value.get_value<std::string *>())->c_str()));
	else {
		return 0;
	}
	return 1;
}

int LuaBinding::luaValueToC(lua_State *L, int id, const rttr::type &param, std::vector<rttr::argument> &args, std::vector<void *> &LuaValue)
{
	int luaType = lua_type(L, id);
	int i = id - 1;

	switch (luaType)
	{

	case LUA_TNUMBER:
		if (param == rttr::type::get<unsigned long>()) {
			unsigned long *value = (unsigned long *)malloc(sizeof(unsigned long));
			*value = (unsigned long)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (unsigned long)*value;
		}
		else if (param == rttr::type::get<unsigned int>()) {
			unsigned int *value = (unsigned int *)malloc(sizeof(unsigned int));
			*value = (unsigned int)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (unsigned int)*value;
		}
		else if (param == rttr::type::get<unsigned short>()) {
			unsigned short *value = (unsigned short *)malloc(sizeof(unsigned short));
			*value = (unsigned short)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (unsigned short)*value;
		}
		else if (param == rttr::type::get<unsigned char>()) {
			unsigned char *value = (unsigned char *)malloc(sizeof(unsigned char));
			*value = (unsigned char)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (unsigned char)*value;
		}
		else if (param == rttr::type::get<long>()) {
			long *value = (long *)malloc(sizeof(long));
			*value = (long)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (long)*value;
		}
		else if (param == rttr::type::get<int>()) {
			int *value = (int *)malloc(sizeof(int));
			*value = (int)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (int)*value;
		}
		else if (param == rttr::type::get<short>()) {
			short *value = (short *)malloc(sizeof(short));
			*value = (short)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (short)*value;
		}
		else if (param == rttr::type::get<char>()) {
			char *value = (char *)malloc(sizeof(char));
			*value = (char)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (char)*value;
		}
		else if (param == rttr::type::get<float>()) {
			float *value = (float *)malloc(sizeof(float));
			*value = (float)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (float)*value;
		}
		else if (param == rttr::type::get<double>()) {
			double *value = (double *)malloc(sizeof(double));
			*value = (double)lua_tonumber(L, id);
			LuaValue[i] = value;
			args[i] = (double)*value;
		}
		else {
			luaL_error(L, "Argument #%d Unknown conversion LuaNumber to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		}
		return 1;
	case LUA_TSTRING:
		if (param == rttr::type::get<char *>()) {
			char *value = strcopy(lua_tostring(L, id));
			LuaValue[i] = value;
			args[i] = (char *)value;
		}
		else if (param == rttr::type::get<const char *>()) {
			char const *value = strcopy(lua_tostring(L, id));
			LuaValue[i] = 0;
			args[i] = (char const *)value;
		}
		else if (param == rttr::type::get<std::string>()) {
			std::string *value = new std::string(lua_tostring(L, id));
			LuaValue[i] = value;
			args[i] = (std::string)*value;
		}
		else {
			luaL_error(L, "Argument #%d Unknown conversion LuaString to %s\n", id, param.get_name().to_string().c_str());
			return 0;
		}
		return 1;
	case LUA_TBOOLEAN:
		if (param == rttr::type::get<bool>()) {
			bool *value = (bool *)malloc(sizeof(bool));
			*value = (bool)lua_toboolean(L, id);
			LuaValue[i] = value;
			args[i] = (bool)*value;
		}
		else {
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