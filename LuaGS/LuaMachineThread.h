#pragma once

#include <iomanip>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "Lua/lua.hpp"

class LuaMachineThread
{
public:
	LuaMachineThread(lua_State *thread);
	virtual ~LuaMachineThread();
	void start();
	void stop();
	void queueEvent(SDL_Event &event);

private:
	lua_State *thread;
	std::queue<SDL_Event> event_queue;
	bool running;
};

