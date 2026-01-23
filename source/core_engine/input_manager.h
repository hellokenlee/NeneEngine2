/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>
#include "core/event_publish_subscribe.h"
#include "core/core.h"

enum class key_event_type : uint8_t
{
	on_key_pressed,
	on_key_released,
	on_key_repeated,
};

enum class key_modifier : uint8_t
{
	none	= 0b00000000,
	ctrl	= 0b00000001,
	alt		= 0b00000010,
	shift	= 0b00000100,
	caps	= 0b00001000,
	num		= 0b00010000,
	sys		= 0b00100000, // key `⌘` in mac; key `⊞` in windows;
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(key_modifier);

struct keyboard_event : event
{
	char m_key;
	key_event_type m_type;
	key_modifier m_modifier;
};

enum class mouse_event_type : uint8_t
{
	on_mouse_move,
	on_mouse_wheel,
	on_mouse_button,
};

struct mouse_event : event
{
	mouse_event_type m_type;
	uint8_t m_button_index;
	int16_t m_delta_scroll;
	float2 m_delta_motion;
	float2 m_position_in_window;
};

class NENE_API input_manager : public event_publisher
{
public:
	static input_manager& instance();
};