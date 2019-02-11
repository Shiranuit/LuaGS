#include <iostream>
#include "Engine.h"

int main(int ac, char **av)
{
	Engine engine(800, 600, "LuaGS");
	engine.run(ac, av);
	return 0;
}