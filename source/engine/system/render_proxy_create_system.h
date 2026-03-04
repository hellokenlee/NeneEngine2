/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <entt/entt.hpp>
#include "core/core.h"


namespace nene::g
{
	class NENE_API static_mesh_render_data_create_system
	{
	public:
		void update(std::chrono::milliseconds delta, entt::registry& registry) const;
	};
}