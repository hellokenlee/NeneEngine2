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

		/** ... add new event here ... */
	};	
}
