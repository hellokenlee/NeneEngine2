/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "win_client.h"
#include "engine/engine_loop.h"
#include "core_object/object.h"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "core_object/meta.h"

int main()
{
	win_client client;

	while (!client.should_exit())
	{
		client.poll_message();
	}
	
	return 0;
}
