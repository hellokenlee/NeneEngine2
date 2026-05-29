/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"
#include "world.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 2)
	{
		py::class_<engine, std::unique_ptr<engine, py::nodelete>>(m, "Engine")
			.def(py::init([]() { return &engine::get(); }))
			.def_static("get_world", []() -> const std::shared_ptr<g::world>& { return engine::get().get_world(); })
		;
	}
}
