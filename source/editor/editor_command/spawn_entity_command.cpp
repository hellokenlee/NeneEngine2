/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "spawn_entity_command.h"
#include "engine/engine_loop.h"
#include "engine/world.h"
#include "engine/component/static_mesh_component.h"
#include "engine/asset/asset_registry.h"


namespace nene
{
	extern logger editor_;
	
	spawn_entity_command::spawn_entity_command(const uuid& asset_uuid)
		: m_asset_uuid(asset_uuid)
	{
	}

	spawn_entity_command::spawn_entity_command(const std::string& asset_path)
		: m_asset_uuid(g::asset_registry::get().find_abstract(asset_path).m_uuid)
	{
		if (m_asset_uuid.is_nil())
		{
			log(editor_, error, "failed to spawn entity: {}", asset_path);
		}
	}

	void spawn_entity_command::execute()
	{
		if (!m_asset_uuid.is_nil())
		{
			// statci mesh
			if (auto handle = g::asset_registry::get().make_handle<g::static_mesh_asset>(m_asset_uuid); handle.is_valid())
			{
				const auto& w = engine_loop::get_world();
				auto factory = w->get_prefab_factory();
				auto instance = w->spawn_entity(factory.m_static_mesh_prefab);
				instance.get_mut<g::static_mesh_component>().m_asset = handle;
				return;
			}
			// other spawnable assets
		}
		log(editor_, error, "failed to spawn entity: {}", uuid_to_string(m_asset_uuid));
	}
}
