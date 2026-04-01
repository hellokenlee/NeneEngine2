/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<spawn_entity_command, editor_command, std::shared_ptr<spawn_entity_command>>(m, "SpawnEntityCommand")
			.def(py::init<uuid>())
			.def(py::init<const std::string&>())
		;
		
		py::class_<remove_entity_command, editor_command, std::shared_ptr<remove_entity_command>>(m, "RemoveEntityCommand")
			.def(py::init<uint64_t>())
		;

		py::class_<parent_entity_command, editor_command, std::shared_ptr<parent_entity_command>>(m, "ParentEntityCommand")
			.def(py::init<uint64_t, uint64_t>())
		;

		py::class_<inspect_entity_command, editor_command, std::shared_ptr<inspect_entity_command>>(m, "InspectEntityCommand")
			.def(py::init<uint64_t>())
		;
	}
}
