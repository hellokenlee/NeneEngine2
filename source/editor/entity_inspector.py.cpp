/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_inspector.h"
#include "core/event_publish_subscribe.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<entity_inspect_event, event, py::smart_holder>(m, "EntityInspectEvent")
		;
		
		py::class_<entity_inspector, event_publisher, std::shared_ptr<entity_inspector>>(m, "EntityInspector")
			.def(py::init([]()
			{
				// Singleton is owned by static storage; Python gets a non-owning shared_ptr view.
				return std::shared_ptr<entity_inspector>(&entity_inspector::get(), [](entity_inspector*) {});
			}))
			.def("inspect", &entity_inspector::inspect)
			.def("get_inspecting_components", &entity_inspector::get_inspecting_components)
		;
	}
}
