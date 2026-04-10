/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_command.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m)
	{
		py::class_<editor_command, py::smart_holder>(m, "EditorCommand")
			.def(py::init<>())
		;
	}
}
