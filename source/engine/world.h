/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "level.h"
#include "prefab_factory.h"
#include "core/event_id.h"
#include "core_render/render_scene.h"
#include "system/camera/camera_control_system.h"
#include "system/camera/main_render_view_extract_system.h"

#include <memory>
#include <flecs.h>


namespace nene::g
{
	/** world related events */
	struct entity_spawn_event : typed_event<event_id::entity_spawn_event>
	{
		entity_spawn_event(uint64_t id, std::string name) : m_id(id), m_name(std::move(name)) {}
		uint64_t m_id;
		std::string m_name;
	};
	
	/** world holds all levels */
	class NENE_API world final : public event_publisher
	{
	public:
		world();
		
		void update(std::chrono::milliseconds delta);
		
		const flecs::world& get_ecs() const { return m_ecs; };
		
		flecs::entity spawn_entity(flecs::entity prefab);
		
		bool remove_entity(const uint64_t& eid);
		
		bool parent_entity(const uint64_t& parent_eid, const uint64_t& child_eid);
		
		const prefab_factory& get_prefab_factory() const { return m_prefab_factory; }
		
		const std::shared_ptr<r::render_scene>& get_render_scene() const;
		const std::shared_ptr<r::render_view>& get_main_render_view() const;
		
		
	private:
		// levels
		std::shared_ptr<level> m_presistent_level;
		std::vector<std::shared_ptr<level>> m_levels;
		
		// entities
		flecs::world m_ecs;
		
		// prefabs
		prefab_factory m_prefab_factory;
		
		// systems
		std::shared_ptr<camera_control_system> m_camera_control_system;
		main_render_view_extract_system m_main_render_view_extract_system;
		
		//
		std::shared_ptr<r::render_scene> m_render_scene;
	};
}
