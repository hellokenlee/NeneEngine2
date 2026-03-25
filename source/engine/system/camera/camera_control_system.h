/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <flecs.h>


namespace nene::g
{
	class NENE_API camera_control_system final : public event_subscriber
	{
	public:
		camera_control_system(flecs::world& ecs);
		void on_notify(const event& e) override;
		
	private:
		//
		bool m_b_operating : 1 = false;
		//
		bool m_b_moving_forward : 1 = false;
		bool m_b_moving_back : 1 = false;
		bool m_b_moving_left : 1 = false;
		bool m_b_moving_right : 1 = false;
		//
		float m_move_speed = 0.1f;
		//
		rotator m_rotator = {};
		float m_sensitivity = 0.3f;
		uint2 m_window_size = {800, 600};
	};
}