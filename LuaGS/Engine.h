#pragma once
#include "Display.h"
#include "LuaContext.h"

extern Display *display;

class Engine
{
public:
	Engine(unsigned int width, unsigned int height, const std::string &title);
	virtual ~Engine();
	void run(int ac, char **av);
	void init(int ac, char **av);
	void draw();
	void processEvent();
	

private:
	LuaContext lua;
	unsigned int width;
	unsigned int height;
};

