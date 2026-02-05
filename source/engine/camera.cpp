/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "camera.h"
#include "core/event_id.h"
#include "input_manager.h"

namespace nene::g
{
	camera::camera()
	{
		input_manager::instance().add_listener(*this);
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
			break;
		default:
			break;
		}
	}
}
