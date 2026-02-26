/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/event_publish_subscribe.h"
#include "core_render/render_view.h"
#include "scene_component.h"


namespace nene::g
{
	class NENE_API camera_component: public scene_component, public event_listener
	{
	public:
		//
		camera_component();
		const std::shared_ptr<r::render_view>& get_render_view() const { return m_render_view; }
		
		// scene_component overrides
		void on_add_to_scene() override;
		void on_remove_from_scene() override;
		
		// event_listener overrides
		void on_notify(const event& e) override;

	private:
		void update_render_view();
		
		// location
		float3 m_location;
		// rotation
		rotator m_rotator;
		float3 m_cached_right;
		float3 m_cached_forward;
		bool m_b_rotating : 1 = false;
		//
		float m_sensitivity;
		float m_move_speed;
		float m_aspect_ratio;
		
		std::shared_ptr<r::render_view> m_render_view = {};
	};
}
