/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "transform_component.h"
#include "core_object/py_ecs.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<local_transform_component, py::smart_holder>(m, "LocalTransformComponent")
			.def(py::init<>())
			.def_readwrite("m_location", &local_transform_component::m_location)
			.def_readwrite("m_rotation", &local_transform_component::m_rotation)
			.def_readwrite("m_scale", &local_transform_component::m_scale)
		;
		
		ecs::register_component_type<local_transform_component>();
	}
}
