#include "EngineVariables.h"

LuaContext EngineVariables::lua;
lua_State *EngineVariables::main_thread;
Display *EngineVariables::display = NULL;
FontRenderer *EngineVariables::fontRenderer = NULL;
unsigned int EngineVariables::width = 800;
unsigned int EngineVariables::height = 600;

