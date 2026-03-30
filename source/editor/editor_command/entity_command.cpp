/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_command.h"
#include "engine/asset/asset_registry.h"
#include "engine/component/static_mesh_component.h"
#include "engine/engine_loop.h"
#include "engine/world.h"


namespace nene
{
	extern logger editor_;

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
				instance.set<g::static_mesh_component>({handle});
				return;
			}
			// other spawnable assets
		}
		log(editor_, error, "failed to spawn entity: {}", uuid_to_string(m_asset_uuid));
	}
	
	void remove_entity_command::execute()
	{
		const auto& w = engine_loop::get_world();
		if (w == nullptr)
		{
			log(editor_, error, "failed to remove entity {}: world is null", m_eid);
			return;
		}
		
		if (!w->remove_entity(m_eid))
		{
			log(editor_, error, "failed to remove entity {}", m_eid);
		}
	}

	void parent_entity_command::execute()
	{
		const auto& w = engine_loop::get_world();
		if (w == nullptr)
		{
			log(editor_, error, "failed to parent entity {} to {}: world is null", m_child_eid, m_parent_eid);
			return;
		}
		
		if (!w->parent_entity(m_parent_eid, m_child_eid))
		{
			log(editor_, error, "failed to parent entity {} to {}", m_child_eid, m_parent_eid);
		}
	}
}
