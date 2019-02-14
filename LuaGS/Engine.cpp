#include "Engine.h"
#include "LuaBinding.h"
#include "LuaGL.h"


void Engine::Setup(unsigned int _width, unsigned int _height, const std::string &title)
{
	EngineVariables::display = new Display(_width, _height, title);
	EngineVariables::fontRenderer = new FontRenderer("assets/term_font.png", _width, _height);
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

	LuaGL::LoadLuaGL(EngineVariables::lua.getState());
	Terminal::init(EngineVariables::lua.getState());
	OS::init(EngineVariables::lua.getState());

	int state = EngineVariables::lua.doFile("startup.lua");
	if (state != LUA_OK) {
		printf("Error = %s\n", lua_tostring(EngineVariables::lua.getState(), -1));
		exit(84);
	}
}

void Engine::run(int ac, char **av)
{
	init(ac, av);
	while (!EngineVariables::display->isClosed()) {
		EngineVariables::display->event();
	}
}