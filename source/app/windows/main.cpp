/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "windows.h"

#include "win_client.h"
#include "engine/engine_loop.h"

int main()
{
	win_client client;

	engine_loop::init();

	while (!client.should_exit())
	{
		client.poll_messages();
		engine_loop::update();
	}
	
	engine_loop::shutdown();

	return 0;
	
}
