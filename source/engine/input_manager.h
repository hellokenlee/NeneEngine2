/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>
#include "core/event_publish_subscribe.h"
#include "core/core.h"
#include "core/event_id.h"

namespace nene
{
	enum class key_event_type : uint8_t
	{
		none,
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
		os		= 0b00100000, // key `⌘` in mac; key `田` in windows;
	};
	DEFINE_FLAG_ENUM_CLASS_OPERATORS(key_modifier);

	struct keyboard_event : event
	{
		keyboard_event() : event(event_id::keyboard_event) {}
		
		char m_key = 0;
		key_event_type m_type = key_event_type::none;
		key_modifier m_modifier = key_modifier::none;
	};

	enum class mouse_event_type : uint8_t
	{
		none,
		on_mouse_move,
		on_mouse_wheel,
		on_mouse_button,
	};

	struct mouse_event : event
	{
		mouse_event() : event(event_id::mouse_event) {}
		
		mouse_event_type m_type = mouse_event_type::none;
		uint8_t m_button_index = 0;
		key_event_type m_button_type = key_event_type::none;
		int32_t m_delta_scroll = 0;
		float2 m_delta_motion = {};
		float2 m_position_in_window = {};
	};

	class NENE_API input_manager : public event_publisher
	{
	public:
		static input_manager& instance();

	private:
		input_manager() = default;
	};
}
