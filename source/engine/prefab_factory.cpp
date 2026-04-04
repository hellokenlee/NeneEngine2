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
		m_static_mesh_prefab = ecs.prefab("StaticMeshEntity")
			.set<local_transform_component>({})
			.set<world_transform_component>({})
			.set<static_mesh_component>({})
		;
		
		//
		m_camera_prefab = ecs.prefab("CameraEntity")
			.set<local_transform_component>({})
			.set<world_transform_component>({})
			.set<camera_component>({})
			.add<controlling_camera_tag>()
		;
		
		//
		(void)ecs.component<main_camera_relation>().add(flecs::Exclusive);
		ecs.add<main_camera_relation>(m_camera_prefab); 
	}
}
