/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "transform_update_system.h"
#include "component/transform_component.h"


namespace nene::g
{
	transform_update_system::transform_update_system(flecs::world& ecs)
	{
		// <entity world transform - readwrite, entity local transform - readonly, parent world transform - readonly>
		ecs.system<world_transform_component, const local_transform_component, const world_transform_component*>()
			// the 3rd param ( `parent world transform` ) is: 1. from parent; 2. update by order; 3. optional 
			.term_at(3).parent().cascade().optional()
			.each(
				[](flecs::entity e, world_transform_component& world, const local_transform_component& local, const world_transform_component* parent_world)
				{
					auto m = matrix::make_translation_matrix(local.m_location) * matrix::make_rotation_matrix(local.m_rotation) * matrix::make_scale_matrix(local.m_scale);
					if (parent_world)
					{
						world.m_world_matrix = parent_world->m_world_matrix * m;
					}
					else
					{
						world.m_world_matrix = m;
					}
				}
			)
		;
		
	}
}
