/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "win_client.h"
#include "engine/engine_loop.h"
#include "core_object/object.h"

int main()
{
	win_client client;
	
	engine_loop::init(client.get_window());

	while (!client.should_exit())
	{
		client.poll_messages();
		engine_loop::update();
	}
	
	engine_loop::shutdown();

	return 0;
	
}
