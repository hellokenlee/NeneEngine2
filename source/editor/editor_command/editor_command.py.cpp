/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_import_command.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m)
	{
		py::class_<editor_command, std::shared_ptr<editor_command>>(m, "EditorCommand")
			.def(py::init<>())
		;
	}
}
