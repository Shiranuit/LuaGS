#include "Engine.h"
#include "LuaBinding.h"
#include "LuaGL.h"

Engine::Engine(unsigned int _width, unsigned int _height, const std::string &title) : width(_width), height(_height)
{
	display = new Display(_width, _height, title);
	fontRenderer = new FontRenderer("assets/term_font.png", _width, _height);
}


Engine::~Engine()
{
	delete display;
	delete fontRenderer;
}

void Engine::init(int ac, char **av)
{
	display->clear(0.f, 0.f, 0.f, 1.f);
	display->sync();

	LuaGL::LoadLuaGL(lua.getState());
	Terminal::initTerminal(lua.getState());

	int state = lua.doFile("startup.lua");
	if (state != LUA_OK) {
		printf("Error = %s\n", lua_tostring(lua.getState(), -1));
		exit(84);
	}
}

void Engine::processEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {

		}
	}
}

void Engine::draw()
{
	
}


void Engine::run(int ac, char **av)
{
	init(ac, av);
	while (!display->isClosed()) {
		processEvent();
		
	}
	
}