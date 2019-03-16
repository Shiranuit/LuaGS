#include "Engine.h"
#include "LuaBinding.h"
#include "LuaGL.h"


void Engine::Setup(unsigned int _width, unsigned int _height, const std::string &title)
{
	EngineVariables::display = new Display(_width, _height, title);
	EngineVariables::fontRenderer = new FontRenderer("assets/term_font.png", _width, _height);
	EngineVariables::width = _width;
	EngineVariables::height = _height;
}


void Engine::destroy()
{
	delete EngineVariables::display;
	delete EngineVariables::fontRenderer;
}

void Engine::init(int ac, char **av)
{
	EngineVariables::display->clear(0.f, 0.f, 0.f, 1.f);
	EngineVariables::display->sync();
	EngineVariables::main_thread = lua_newthread(EngineVariables::lua.getState());
	luaL_openlibs(EngineVariables::main_thread);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DOUBLEBUFFER);
	glDepthRange(0.0f, 1.0f);

	LuaGL::LoadLuaGL(EngineVariables::lua.getState());
	Terminal::init(EngineVariables::lua.getState());
	OS::init(EngineVariables::lua.getState());
	FileSystem::init(EngineVariables::lua.getState());

	int state = luaL_loadfile(EngineVariables::main_thread, "startup.lua");
	if (state != LUA_OK) {
		printf("Error = %s\n", lua_tostring(EngineVariables::main_thread, -1));
	}
}

void Engine::run(int ac, char **av)
{
	std::cout << "Path: " << av[0] << std::endl;
	init(ac, av);
	int state = LUA_YIELD;
	while (!EngineVariables::display->isClosed()) {
		state = lua_status(EngineVariables::main_thread);
		if (state == LUA_OK || state == LUA_YIELD) {
			int val = lua_resume(EngineVariables::main_thread, EngineVariables::lua.getState(), OS::pullEvent(EngineVariables::main_thread));
			if (!(val == LUA_OK || val == LUA_YIELD)) {
				printf("Error = %s\n", lua_tostring(EngineVariables::main_thread, -1));
			}
		} else {
			EngineVariables::display->event();
		}
	}
}