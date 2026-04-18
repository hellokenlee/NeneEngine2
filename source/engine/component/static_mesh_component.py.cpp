/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "static_mesh_component.h"
#include "core_object/py_ecs.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::classh<static_mesh_component>(m, "StaticMeshComponent")
			.def(py::init<>())
			.def_readwrite("m_mesh", &static_mesh_component::m_mesh)
			.def_readwrite("m_material", &static_mesh_component::m_material)
		;
		
		ecs::register_component_type<static_mesh_component>();
	}
}
