#include "LuaMachineThread.h"



LuaMachineThread::LuaMachineThread(lua_State *thread)
{
	this->thread = thread;
}


LuaMachineThread::~LuaMachineThread()
{
	int size = event_queue.size();
	for (int i = 0; i < size; i++) {
		event_queue.pop();
	}
}

void LuaMachineThread::start()
{
	running = true;
	while (running && this->thread != NULL) {
		if (event_queue.size() > 0) {
			SDL_Event event = event_queue.front();
			std::thread pthread([event]() {
				
			});
			event_queue.pop();
		}
	}
}

void LuaMachineThread::stop()
{
	running = false;
}

void LuaMachineThread::queueEvent(SDL_Event &event)
{

}