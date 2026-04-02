/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "world.h"

#include "input_manager.h"
#include "level.h"
#include "component/editor_component.h"
#include "component/camera_component.h"
#include "core_object/py.h"


namespace nene::g
{
	logger world_("world");
	
	world::world()
		: m_presistent_level(nullptr)
		, m_prefab_factory(m_ecs)
		, m_camera_control_system(std::make_shared<camera_control_system>(m_ecs))
		, m_main_render_view_extract_system(m_ecs)
		, m_render_scene(std::make_shared<r::render_scene>())
	{
		binding::get().call_ecs_register_functions(m_ecs);
		input_manager::get().add_subscriber(m_camera_control_system);
	}

	void world::update(std::chrono::milliseconds delta)
	{
		// TODO: level updates
		
		// system updates
		m_ecs.progress(std::chrono::duration<float>(delta).count());
	}

	flecs::entity world::spawn_entity(flecs::entity prefab)
	{
		// maybe it's editor-only code
		std::string entity_name = {};
		{
			if (!prefab.has<spawn_name_counter_component>())
			{
				prefab.set<spawn_name_counter_component>({1});
			}
			const auto& count = prefab.get<spawn_name_counter_component>().m_count;
			entity_name = std::string(prefab.name()) + "_" + std::to_string(count);
			prefab.set<spawn_name_counter_component>({count + 1});
		}
		
		
		// 
		auto result = m_ecs.entity(entity_name.c_str()).is_a(prefab);
		
		// maybe it's editor-only code
		{
			log(world_, info, "spawn entity: {}", result.id());
			entity_spawn_event e(result.id(), std::string(result.name()));
			notify(e);
		}
		
		return result;
	}

	bool world::remove_entity(const uint64_t& eid)
	{
		if (eid == 0)
		{
			return false;
		}
		
		flecs::entity entity = m_ecs.entity(static_cast<flecs::entity_t>(eid));
		if (!entity.is_alive())
		{
			return false;
		}
		
		entity.destruct();
		return true;
	}
	
	bool world::parent_entity(const uint64_t& parent_eid, const uint64_t& child_eid)
	{
		if (parent_eid == 0 || child_eid == 0)
		{
			return false;
		}
		flecs::entity parent = m_ecs.entity(static_cast<flecs::entity_t>(parent_eid));
		flecs::entity child = m_ecs.entity(static_cast<flecs::entity_t>(child_eid));
		child.child_of(parent);
		return true;
	}

	void world::set_main_camera_entity(const flecs::entity& e)
	{
		CHECK(e.has<camera_component>());
		m_ecs.add<main_camera_relation>(e);
	}

	const std::shared_ptr<r::render_scene>& world::get_render_scene() const
	{
		return m_render_scene;
	}

	const std::shared_ptr<r::render_view>& world::get_main_render_view() const
	{
		return m_main_render_view_extract_system.get_render_view();
	}
}
