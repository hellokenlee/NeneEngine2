/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "scene.h"
#include "component/transform_component.h"
#include "component/camera_component.h"
#include "component/static_mesh_component.h"
#include "asset/asset_registry.h"
#include "input_manager.h"


namespace nene::g
{
	scene::scene()
	{
		// temp test code
		{
			// create the main camera
			auto main_camera_entity = m_registery.create();
			m_registery.emplace<transform_component>(main_camera_entity);
			m_registery.emplace<camera_component>(main_camera_entity);
			m_registery.emplace<main_controlling_camera_tag>(main_camera_entity);
			
			// mark as the main camera
			m_registery.ctx().emplace<main_camera_entity_relation>(main_camera_entity);
			
			
			//
			auto test_static_mesh_entity = m_registery.create();
			m_registery.emplace<static_mesh_component>(test_static_mesh_entity);
		}
		// temp test code
		m_camera_control_system = std::make_shared<camera_control_system>();
		input_manager::get().add_listener(m_camera_control_system);
	}

	void scene::update(std::chrono::milliseconds delta)
	{
		m_camera_control_system->update(delta, m_registery);
		m_main_render_view_extract_system.update(delta, m_registery);
	}

	const std::shared_ptr<r::render_view>& scene::get_main_render_view() const
	{
		return m_main_render_view_extract_system.get_render_view();
	}
}
