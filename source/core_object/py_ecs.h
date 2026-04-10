/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "py.h"
#include <flecs.h>


namespace nene::ecs
{
	/** connect py::type <-> flecs::component::id */
	template<typename cpp_t>
	void register_component_type()
	{
		g::binding::get().add_ecs_register_function(
			[](const flecs::world& ecs)
			{
				auto cid = ecs.component<cpp_t>().id();
				auto cls = py::type::of<cpp_t>();
				g::binding::get().set_ecs_component_type(cid, cls);
			}
		);
	}
}