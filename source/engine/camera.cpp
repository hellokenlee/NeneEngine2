/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "camera.h"
#include "core/event_id.h"
#include "input_manager.h"
#include "core_render/render_thread.h"

namespace nene::g
{
	camera::camera()
		: m_location(0.0, 0.0, 0.0)
		, m_rotator(0.0, 0.0, 0.0)
	{
		// TODO: create render resource when added to scene
		enqueue_render_command<__func__>(
			[this, view_location = m_location, view_rotator = m_rotator]()
			{
				m_render_view = std::make_shared<r::render_view>();
				m_render_view->update_view_matrix(view_location, view_rotator);
				m_render_view->set_perspective_projection_matrix();
			}
		);
	}

	void camera::on_notified(const event& e)
	{
		switch (e.m_id)
		{
		case event_id::keyboard_event:
			{
				const auto& ee = dynamic_cast<const keyboard_event&>(e);
				break;
			}
		case event_id::mouse_event:
			{
				break;	
			}
		default:
			break;
		}
	}
}
