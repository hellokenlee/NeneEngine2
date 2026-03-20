/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_command_center.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m)
	{
		py::class_<editor_command_center, std::unique_ptr<editor_command_center, py::nodelete>>(m, "EditorCommandCenter")
			.def(py::init([]() { return &editor_command_center::get(); }))
			.def("invoke", &editor_command_center::invoke)
		;
	}
}
