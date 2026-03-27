/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_command_center.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<editor_command_center, std::unique_ptr<editor_command_center, py::nodelete>>(m, "EditorCommandCenter")
			.def(py::init([]() { return &editor_command_center::get(); }))
			.def("invoke", &editor_command_center::invoke)
			.def("undo", &editor_command_center::undo)
			.def("redo", &editor_command_center::redo)
			.def("can_undo", &editor_command_center::can_undo)
			.def("can_redo", &editor_command_center::can_redo)
			.def("clear_history", &editor_command_center::clear_history)
		;
	}
}
