/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "win_client.h"
#include "engine/engine_loop.h"
#include "core_object/object.h"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "core_object/meta.h"

int main()
{
	/*
	win_client client;
	
	engine_loop::init(client.get_window());

	while (!client.should_exit())
	{
		client.poll_messages();
		engine_loop::update();
	}
	
	engine_loop::shutdown();
	//*/

	foo();

	pybind11::scoped_interpreter guard{};
	
	auto content_module = pybind11::module::import("content.scripts.main");
	content_module.attr("main")();
	return 0;
	
}
