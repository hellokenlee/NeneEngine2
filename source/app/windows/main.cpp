/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "windows.h"

#include "client.h"
#include "engine/engine_loop.h"

int main()
{
	client client;

	engine_loop::initialize();

	while (!client::s_should_exit)
	{
		client.poll_events();
		engine_loop::update();
	}
	
	engine_loop::shutdown();

	return 0;
	
}
