/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "prefab_factory.h"
#include "component/transform_component.h"
#include "component/static_mesh_component.h"


namespace nene::g
{
	prefab_factory::prefab_factory(flecs::world& ecs)
	{
		m_static_mesh_prefab = ecs.prefab("StaticMeshEntity")
			.set<transform_component>({})
			.set<static_mesh_component>({})
		;
	}
}
