/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_command_center.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m)
	{
		py::class_<editor_command_center>(m, "EditorCommandCenter")
			.def_static("get", &editor_command_center::get, py::return_value_policy::reference)
			.def("invoke", &editor_command_center::invoke)
		;
	}
}
