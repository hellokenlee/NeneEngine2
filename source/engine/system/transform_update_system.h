/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <flecs.h>


namespace nene::g
{
	class NENE_API transform_update_system
	{
	public:
		transform_update_system(flecs::world& ecs);
	};
}