/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "spawn_entity_command.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<spawn_entity_command, editor_command>(m, "SpawnEntityCommand")
			.def(py::init<const uuid&>())
			.def(py::init<const std::string&>())
		;
	}
}
