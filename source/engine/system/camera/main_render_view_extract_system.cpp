/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "main_render_view_extract_system.h"
#include "component/camera_component.h"
#include "core_render/render_thread.h"

namespace nene::g
{
	main_render_view_extract_system::main_render_view_extract_system()
	{
		m_render_view = std::make_shared<r::render_view>();
	}

	void main_render_view_extract_system::update(std::chrono::milliseconds delta, entt::registry& registry)
	{
		if (registry.ctx().contains<main_camera_entity_relation>())
		{
			auto eid = registry.ctx().get<main_camera_entity_relation>().m_eid;
			const auto& cam = registry.get<camera_component>(eid);
			enqueue_render_command<__func__>(
				[render_view = m_render_view, view_location = cam.m_location, view_direction = cam.m_forward, fov = cam.m_fov, ratio = cam.m_ratio, near = cam.m_near, far = cam.m_far]()
				{
					render_view->update_view_matrix(view_location, view_direction);
					render_view->set_perspective_projection_matrix(fov, ratio, near, far);
				}
			);
		}
	}
}
