/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::classh<spawn_entity_command, editor_command>(m, "SpawnEntityCommand")
			.def(py::init<const std::string&>())
		;
		
		py::classh<remove_entity_command, editor_command>(m, "RemoveEntityCommand")
			.def(py::init<uint64_t>())
		;

		py::classh<parent_entity_command, editor_command>(m, "ParentEntityCommand")
			.def(py::init<uint64_t, uint64_t>())
		;
	}
}
