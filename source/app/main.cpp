/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/core.h"
#include "core/log.h"
#include "core/stdout_log_handler.h"
#include "core_object/py.h"
#include "engine/engine_loop.h"
#include "engine/world.h"
#include "engine/asset/asset_registry.h"
#include "engine/component/static_mesh_component.h"

int main()
{
	// Name the main thread inside the Tracy profiler.
	tracy::SetThreadName("MainThread");
	//
	auto handler = std::make_shared<nene::stdout_log_handler>();
	nene::logger::publisher()->add_subscriber(handler);
	//
	auto vm = nene::g::binding::get().initialize();
	// create client
	nene::client runtime_client;
	if (runtime_client.get_window() == nullptr)
	{
		return -1;
	}
	// main loop
	while (!runtime_client.should_exit())
	{
		runtime_client.update();
		FrameMark;
	}
	return 0;
}