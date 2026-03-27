/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/event_publish_subscribe.h"
#include "core_render/render_view.h"


namespace nene::g
{
	/** game thread camera info */
	struct camera_component
	{
		// perspective
		float m_fov = 45.0f;
		float m_ratio = 1.0f;
		
		// orthographic
		float m_width;
		float m_height;
		
		//
		float m_near = 0.01f;
		float m_far = 10000.0f;
		
		//
		bool m_b_is_perspective = true;
		
		float3 m_location = float3::zero();
		float3 m_up = float3::up_vector();
		float3 m_right = float3::right_vector();
		float3 m_forward = float3::forward_vector();
	};
	
	/** the choose a camera to control & render */
	struct main_camera_tag {};
}
