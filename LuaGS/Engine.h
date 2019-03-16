#pragma once
#include "EngineVariables.h"
#include "Terminal.h"
#include "OS.h"
#include "FileSystem.h"

#include <mutex>
#include <thread>

class Engine
{
public:
	static void Setup(unsigned int width, unsigned int height, const std::string &title);
	static void destroy();
	static void run(int ac, char **av);
	static void init(int ac, char **av);

};

