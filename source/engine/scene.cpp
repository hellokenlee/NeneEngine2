/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "scene.h"
#include "components/transform_component.h"
#include "components/camera_component.h"
#include "input_manager.h"

namespace nene::g
{
	scene::scene()
	{
		{
			// create main camera
			auto main_camera_entity = m_registery.create();
			m_registery.emplace<transform_component>(main_camera_entity);
			m_registery.emplace<camera_component>(main_camera_entity);
			m_registery.emplace<main_controling_camera_tag>(main_camera_entity);
			
			// mark as main camera
			m_registery.ctx().emplace<main_camera_entity_relation>(main_camera_entity);
		}
		// TODO: 
		m_camera_control_system = std::make_shared<camera_control_system>();
		input_manager::get().add_listener(m_camera_control_system);
	}

	void scene::update(std::chrono::milliseconds delta)
	{
		m_camera_control_system->update(delta, m_registery);
		m_main_render_view_extract_system.update(delta, m_registery);
	}

	std::shared_ptr<r::render_view> scene::get_main_render_view()
	{
		return m_main_render_view_extract_system.get_render_view();
	}
}
