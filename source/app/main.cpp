/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/log.h"
#include "core/stdout_log_handler.h"
#include "core_object/py.h"
#include "engine/engine_loop.h"
#include "engine/world.h"
#include "engine/asset/asset_registry.h"
#include "engine/component/static_mesh_component.h"

int main()
{
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
	
	// statci mesh
	nene::uuid asset_uuid = nene::string_to_uuid("1933e836-e594-4a2a-ace6-4e0d0ad23ae4");
	if (auto handle = nene::g::asset_registry::get().make_handle<nene::g::static_mesh_asset>(asset_uuid); handle.is_valid())
	{
		const auto& w = nene::engine_loop::get_world();
		auto factory = w->get_prefab_factory();
		auto instance = w->spawn_entity(factory.m_static_mesh_prefab);
		instance.set<nene::g::static_mesh_component>({handle});
	}
	// main loop
	while (!runtime_client.should_exit())
	{
		runtime_client.update();
	}
	return 0;
}