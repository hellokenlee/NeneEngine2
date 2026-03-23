/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <flecs.h>


namespace nene::g
{
	class NENE_API static_mesh_render_data_create_system
	{
	public:
		static_mesh_render_data_create_system(flecs::world& world);
	};
}