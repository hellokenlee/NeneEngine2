/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_command.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m)
	{
		py::classh<editor_command>(m, "EditorCommand")
			.def(py::init<>())
		;
	}
}
