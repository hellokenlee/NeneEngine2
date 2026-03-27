/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "prefab_factory.h"

#include "component/camera_component.h"
#include "component/transform_component.h"
#include "component/static_mesh_component.h"


namespace nene::g
{
	prefab_factory::prefab_factory(flecs::world& ecs)
	{
		// 
		ecs.component<main_camera_tag>().add(flecs::Exclusive);
		
		// 
		m_static_mesh_prefab = ecs.prefab("StaticMeshEntity")
			.set<transform_component>({})
			.set<static_mesh_component>({})
		;
		
		//
		m_camera_prefab = ecs.prefab("CameraEntity")
			.set<transform_component>({})
			.set<camera_component>({})
			.add<main_camera_tag>()
		;
	}
}
