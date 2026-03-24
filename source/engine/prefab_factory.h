/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "flecs.h"


namespace nene::g
{
	struct prefab_factory
	{
		prefab_factory(flecs::world& ecs);
		
		flecs::entity m_static_mesh_prefab;
	};
}