#include <iostream>
#include "Engine.h"

int main(int ac, char **av)
{
	Engine::Setup(1200, 720, "LuaGS");
	Engine::run(ac, av);
	Engine::destroy();
	return 0;
}