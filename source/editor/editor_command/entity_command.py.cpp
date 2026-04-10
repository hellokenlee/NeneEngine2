/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<spawn_entity_command, editor_command, py::smart_holder>(m, "SpawnEntityCommand")
			.def(py::init<const std::string&>())
		;
		
		py::class_<remove_entity_command, editor_command, py::smart_holder>(m, "RemoveEntityCommand")
			.def(py::init<uint64_t>())
		;

		py::class_<parent_entity_command, editor_command, py::smart_holder>(m, "ParentEntityCommand")
			.def(py::init<uint64_t, uint64_t>())
		;
	}
}
