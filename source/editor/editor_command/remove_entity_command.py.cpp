/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "remove_entity_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<remove_entity_command, editor_command>(m, "RemoveEntityCommand")
			.def(py::init<const uint64_t&>())
		;
	}
}
