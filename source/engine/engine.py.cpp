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
			.def("get_world", &engine::get_world)
			.def_static("shutdown", &engine::shutdown)
		;
	}
}
