/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "world.h"

#include "input_manager.h"
#include "level.h"
#include "component/camera_component.h"
#include "component/transform_component.h"


namespace nene::g
{
	world::world()
		: m_presistent_level(nullptr)
		, m_prefab_factory(m_ecs)
		, m_camera_control_system(std::make_shared<camera_control_system>(m_ecs))
		, m_main_render_view_extract_system(m_ecs)
	{
		//
		m_ecs.component<main_rendering_camera_tag>().add(flecs::Exclusive);
		// 
		{
			auto main_camera_entity = m_ecs.entity("EditorCameraEntity")
				.set<transform_component>({})
				.set<camera_component>({})
				.add<main_controlling_camera_tag>()
			;
			
			m_ecs.add<main_rendering_camera_tag>(main_camera_entity);
		}
		//
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
		auto result = m_ecs.entity().is_a(prefab);
		
		// maybe it's editor-only code
		{
			entity_spawn_event e;
			e.m_id = result.id();
			e.m_name = result.name();
			notify(e);
		}
		
		return result;
	}

	const std::shared_ptr<r::render_view>& world::get_main_render_view() const
	{
		return m_main_render_view_extract_system.get_render_view();
	}
}
