/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/log.h"
#include "core/log_handler.h"

#include <SDL3/SDL.h>

extern logger client_;

int main()
{
	// 
	logger::add_handler(std::make_shared<stdout_log_handler>());
	//
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		log(client_, log_level::fatal, "failed to init sdl!");
		return -1;
	}
	//
	client runtime_client;
	if (runtime_client.get_window() == nullptr)
	{
		return -1;
	}
	//
	while (!runtime_client.should_exit())
	{
		runtime_client.update();
	}
	return 0;
}