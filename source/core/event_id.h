/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>

enum class event_id : uint32_t
{
	empty_event = 0,
	keyboard_event,
	mouse_event,
};
