/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <entt/entt.hpp>
#include <chrono>

#include "core/core.h"
#include "systems/camera/camera_control_system.h"
#include "systems/camera/main_render_view_extract_system.h"

namespace nene::g
{
	/** basically a collection of entities */
	class NENE_API scene
	{
	public:
		scene();
		void update(std::chrono::milliseconds delta);
		std::shared_ptr<r::render_view> get_main_render_view();
		
	protected:
		//
		entt::registry m_registery;
		
		// systems
		std::shared_ptr<camera_control_system> m_camera_control_system;
		main_render_view_extract_system m_main_render_view_extract_system;
	};
}