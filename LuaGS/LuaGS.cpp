#include <iostream>
#include "Display.h"

int main(int ac, char **av)
{
	Display disp(800, 600, "LuaGL");

	while (!disp.isClosed()) {
		disp.event();
	}
	return 0;
}