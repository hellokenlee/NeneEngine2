/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "camera_control_system.h"
#include "input_manager.h"
#include "components/camera_component.h"

namespace nene::g
{
	void camera_control_system::update(std::chrono::milliseconds delta, entt::registry& registry) const
	{
		auto view = registry.view<camera_component, main_controling_camera_tag>();
		for (auto [eid, cam] : view.each())
		{
			// update rotation
			cam.m_forward = float3::make_forward_vector(m_rotator);
			cam.m_right = cam.m_up.cross(cam.m_forward);
				
			// update location 
			if (m_b_moving_forward)
			{
				cam.m_location += (cam.m_forward * m_move_speed);
			}
			if (m_b_moving_back)
			{
				cam.m_location -= (cam.m_forward * m_move_speed);
			}
			if (m_b_moving_right)
			{
				cam.m_location += (cam.m_right * m_move_speed);
			}
			if (m_b_moving_left)
			{
				cam.m_location -= (cam.m_right * m_move_speed);
			}
			
			// update aspect ratio
			cam.m_ratio = static_cast<float>(m_window_size.w) / static_cast<float>(m_window_size.h);
			cam.m_width = static_cast<float>(m_window_size.w);
			cam.m_height = static_cast<float>(m_window_size.h);
		}
	}

	void camera_control_system::on_notify(const event& e)
	{
		switch (e.m_id)
		{
		case event_id::keyboard_event:
			{
				if (m_b_operating)
				{
					const auto& ee = static_cast<const keyboard_event&>(e);
					if (ee.m_type == key_event_type::on_key_press || ee.m_type == key_event_type::on_key_release)
					{
						switch (ee.m_key)
						{
						case 'w':
							m_b_moving_forward = ee.m_type == key_event_type::on_key_press;
							break;
						case 's':
							m_b_moving_back = ee.m_type == key_event_type::on_key_press;
							break;
						case 'a':
							m_b_moving_left = ee.m_type == key_event_type::on_key_press;
							break;
						case 'd':
							m_b_moving_right = ee.m_type == key_event_type::on_key_press;
							break;
						default:
							break;
						}
					}
				}
				break;
			}
		case event_id::mouse_event:
			{
				const auto& ee = static_cast<const mouse_event&>(e);
				switch (ee.m_type)
				{
					case mouse_event_type::on_mouse_move:
						{
							if (m_b_operating && !ee.m_delta_motion.equals(float2::zero()))
							{
								m_rotator.pitch += ee.m_delta_motion.y * m_sensitivity;
								m_rotator.yaw += ee.m_delta_motion.x * m_sensitivity;
							}
							break;
						}
					case mouse_event_type::on_mouse_wheel:
						{
							break;
						}
					case mouse_event_type::on_mouse_button:
						{
							if (ee.m_button_index == mouse_event::MOUSE_BUTTON_RIGHT)
							{
								m_b_operating = ee.m_button_type == key_event_type::on_key_press;
							}
						}
					default:
						break;
				}
				break;	
			}
		case event_id::window_resize_event:
			{
				const auto& ee = static_cast<const windows_resize_event&>(e);
				m_window_size = ee.m_new_window_size;
				break;
			}
		default:
			break;
		}
	}
}
