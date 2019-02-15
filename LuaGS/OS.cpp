#include "OS.h"
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

void OS::init(lua_State *L)
{
	if (L == nullptr) {
		std::cout << "Error Loading OSAPI" << std::endl;
		exit(84);
	}
	lua_getglobal(L, "os");
	lua_pushvalue(L, -1);

	LuaValueHelper::pushCFuncToTable(L, "timeNano", timeNano);
	LuaValueHelper::pushCFuncToTable(L, "pullEvent", pullEvent);
	LuaValueHelper::pushCFuncToTable(L, "pathExec", pathExec);
	SDL_StartTextInput();
}

int OS::timeNano(lua_State *L)
{
	if (L == nullptr)
		return 0;
	auto start = std::chrono::high_resolution_clock::now();
	
	lua_pushinteger(L, start.time_since_epoch().count());
	return 1;
}

int OS::pathExec(lua_State *L)
{
	if (L == nullptr)
		return 0;

	WCHAR buffer[_MAX_PATH];
	char result[_MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, buffer, _MAX_PATH, result, _MAX_PATH, NULL, NULL);
	std::string::size_type pos = std::string(result).find_last_of("\\/");
	std::string str = std::string(result).substr(0, pos);

	lua_pushstring(L, str.c_str());
	return 1;
}

int OS::pullEvent(lua_State *L)
{
	if (L == nullptr)
		return 0;
	SDL_Event event;
	if (!SDL_PollEvent(&event))
		return 0;

	lua_newtable(L);

	switch (event.type)
	{
	case SDL_QUIT:
		SDL_Quit();
		exit(0);
		break;
	case SDL_KEYDOWN:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "key");
		lua_settable(L, -3);
		
		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.key.keysym.scancode);
		lua_settable(L, -3);
		
		lua_pushinteger(L, 3);
		lua_pushboolean(L, event.key.repeat > 0);
		lua_settable(L, -3);
		break;
	case SDL_KEYUP:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "key_up");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.key.keysym.scancode);
		lua_settable(L, -3);
		break;
	case SDL_MOUSEMOTION:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "mouse_move");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.motion.x);
		lua_settable(L, -3);

		lua_pushinteger(L, 3);
		lua_pushinteger(L, event.motion.y);
		lua_settable(L, -3);

		lua_pushinteger(L, 4);
		lua_pushinteger(L, event.motion.xrel);
		lua_settable(L, -3);

		lua_pushinteger(L, 5);
		lua_pushinteger(L, event.motion.yrel);
		lua_settable(L, -3);
		break;
	case SDL_MOUSEBUTTONDOWN:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "mouse_click");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.button.button);
		lua_settable(L, -3);

		lua_pushinteger(L, 3);
		lua_pushboolean(L, event.button.clicks == 2);
		lua_settable(L, -3);

		lua_pushinteger(L, 4);
		lua_pushinteger(L, event.button.x);
		lua_settable(L, -3);

		lua_pushinteger(L, 5);
		lua_pushinteger(L, event.button.y);
		lua_settable(L, -3);
		break;
	case SDL_MOUSEBUTTONUP:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "mouse_release");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.button.button);
		lua_settable(L, -3);

		lua_pushinteger(L, 3);
		lua_pushinteger(L, event.button.x);
		lua_settable(L, -3);

		lua_pushinteger(L, 4);
		lua_pushinteger(L, event.button.y);
		lua_settable(L, -3);
		break;
	case SDL_MOUSEWHEEL:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "mouse_wheel");
		lua_settable(L, -3);
		
		lua_pushinteger(L, 2);
		lua_pushinteger(L, event.wheel.x);
		lua_settable(L, -3);

		lua_pushinteger(L, 3);
		lua_pushinteger(L, event.wheel.y);
		lua_settable(L, -3);
		break;
	case SDL_DROPFILE:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "drop_file");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushstring(L, event.drop.file);
		lua_settable(L, -3);

		SDL_free(event.drop.file);
		break;
	case SDL_DROPTEXT:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "drop_text");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushstring(L, event.drop.file);
		lua_settable(L, -3);

		SDL_free(event.drop.file);
		break;
	case SDL_TEXTINPUT:
		lua_pushinteger(L, 1);
		lua_pushstring(L, "char");
		lua_settable(L, -3);

		lua_pushinteger(L, 2);
		lua_pushstring(L, event.text.text);
		lua_settable(L, -3);
		break;
	default:
		break;
	}
	return 1;
}