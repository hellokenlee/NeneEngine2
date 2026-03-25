/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>

namespace nene
{
	enum class event_id : uint16_t
	{
		empty_event = 0,
		keyboard_event,
		mouse_event,
		window_resize_event,
		
		log_message_event,
		
		entity_spawn_event,

		/** ... add new event here ... */
	};	
}
