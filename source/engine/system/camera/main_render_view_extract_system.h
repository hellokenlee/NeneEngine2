/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <entt/entt.hpp>
#include "core_render/render_view.h"


namespace nene::g
{
	class main_render_view_extract_system
	{
	public:
		main_render_view_extract_system();
		void update(std::chrono::milliseconds delta, entt::registry& registry);
		std::shared_ptr<r::render_view> get_render_view() const { return m_render_view; }
		
	private:
		std::shared_ptr<r::render_view> m_render_view = {};
	};
}
