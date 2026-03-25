/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "world.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 2)
	{
		py::class_<engine_loop, std::unique_ptr<engine_loop, py::nodelete>>(m, "EngineLoop")
			.def_static("is_initialized", &engine_loop::is_initialized)
			.def_static("get_world", &engine_loop::get_world)
		;
	}
}
