/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "world.h"
#include "core_object/py.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<entity_spawn_event, event, py::smart_holder>(m, "EntitySpawnEvent")
			.def_readonly("m_id", &entity_spawn_event::m_id)
			.def_readonly("m_name", &entity_spawn_event::m_name)
		;
		
		py::class_<world, event_publisher, py::smart_holder>(m, "World")
		;
	}
}
