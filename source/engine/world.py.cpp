/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "world.h"
#include "core_object/py.h"

namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<entity_spawn_event, event>(m, "EntitySpawnEvent")
			.def_readonly("m_id", &entity_spawn_event::m_id)
			.def_readonly("m_name", &entity_spawn_event::m_name)
		;
		
		// EngineLoop.get_world() returns std::shared_ptr<world>, so bind World with a shared_ptr holder.
		py::class_<world, event_publisher, std::shared_ptr<world>>(m, "World")
		;
	}
}
