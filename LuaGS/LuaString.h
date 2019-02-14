#pragma once
#include <iostream>
class LuaString
{
public:
	LuaString(std::string *_str) : str(_str) {}
	virtual ~LuaString() {}
	operator const char *() { printf("[%s]\n", str->c_str()); return str->c_str(); }
	operator std::string *() { return str; }
	operator std::string() { return *str; }

private:
	std::string *str;
};

