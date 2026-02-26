/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyCppcoreguidelinesProTypeStaticCastDowncast

#include "scene_component/camera_component.h"
#include "core/event_id.h"
#include "input_manager.h"
#include "core_render/render_thread.h"

namespace nene
{
	extern logger engine_;
}

namespace nene::g
{
	camera_component::camera_component()
		: m_location(0.0, 0.0, 0.0)
		, m_rotator(0.0, 0.0, 0.0)
		, m_sensitivity(0.3f)
		, m_move_speed(0.1f)
		, m_aspect_ratio(16.0f / 9.0f)
	{
		m_cached_forward = float3::make_forward_vector(m_rotator);
		m_cached_right = float3::up_vector().cross(m_cached_forward);
	}

	void camera_component::on_add_to_scene()
	{
		// create render thread proxy
		m_render_view = std::make_shared<r::render_view>();
		//
		update_render_view();
	}

	void camera_component::on_remove_from_scene()
	{
		enqueue_render_command<__func__>(
			[render_view = std::move(m_render_view)]() mutable
			{
				render_view.reset();
			}
		);
	}

	void camera_component::on_notify(const event& e)
	{
		bool b_dirty = false;
		switch (e.m_id)
		{
		case event_id::keyboard_event:
			{
				const auto& ee = static_cast<const keyboard_event&>(e);
				switch (ee.m_key)
				{
				case 'w':
					m_location += m_cached_forward * m_move_speed;
					b_dirty = true;
					break;
				case 's':
					m_location -= m_cached_forward * m_move_speed;
					b_dirty = true;
					break;
				case 'a':
					m_location -= (m_cached_right * m_move_speed);
					b_dirty = true;
					break;
				case 'd':
					m_location += (m_cached_right * m_move_speed);
					b_dirty = true;
					break;
				default:
					break;
				}
				break;
			}
		case event_id::mouse_event:
			{
				const auto& ee = static_cast<const mouse_event&>(e);
				if (ee.m_button_index == mouse_event::MOUSE_BUTTON_RIGHT)
				{
					m_b_rotating = ee.m_button_type == key_event_type::on_key_press;
				}
				if (m_b_rotating && !ee.m_delta_motion.equals(float2::zero()))
				{
					m_rotator.pitch += ee.m_delta_motion.y * m_sensitivity;
					m_rotator.yaw += ee.m_delta_motion.x * m_sensitivity;
					m_cached_forward = float3::make_forward_vector(m_rotator);
					m_cached_right = float3::up_vector().cross(m_cached_forward);
					b_dirty = true;
				}
				break;	
			}
		case event_id::window_resize_event:
			{
				const auto& ee = static_cast<const windows_resize_event&>(e);
				float new_aspect = static_cast<float>(ee.m_new_window_size.w) /  static_cast<float>(ee.m_new_window_size.h);
				if (new_aspect != m_aspect_ratio)
				{
					m_aspect_ratio = new_aspect;
					b_dirty = true;	
				}
				break;
			}
		default:
			break;
		}
		
		if (b_dirty)
		{
			update_render_view();
		}
	}

	void camera_component::update_render_view()
	{
		if (m_render_view != nullptr)
		{
			enqueue_render_command<__func__>(
				[render_view = m_render_view, view_location = m_location, view_rotator = m_rotator, aspect = m_aspect_ratio]()
				{
					render_view->update_view_matrix(view_location, view_rotator);
					render_view->set_perspective_projection_matrix(90.0f, aspect, 0.01f, 10000.0f);
				}
			);
		}
	}
}
