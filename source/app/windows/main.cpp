/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "win_client.h"
#include "engine/engine_loop.h"
#include "core_object/object.h"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "core/log_handler.h"
#include "core_object/meta.h"

int main()
{
	//
	logger::add_handler(std::make_shared<stdout_log_handler>());
	//
	win_client client;

	while (!client.should_exit())
	{
		client.poll_message();
	}
	
	return 0;
}
