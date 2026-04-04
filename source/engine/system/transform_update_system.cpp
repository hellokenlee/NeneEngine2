/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "transform_update_system.h"
#include "core/core.h"
#include "component/render_component.h"
#include "component/transform_component.h"
#include "core_render/render_thread.h"


namespace nene::g
{
	transform_update_system::transform_update_system(flecs::world& ecs)
	{
		// <entity world transform - readwrite, entity local transform - readonly, parent world transform - readonly>
		ecs.system<world_transform_component, const local_transform_component, const world_transform_component*>()
			// the 3rd param ( `parent world transform` ) is: 1. from parent; 2. update by order
			.term_at(2).parent().cascade()
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
		
		//
		ecs.system<const render_component&, const world_transform_component&>()
		.each(
			[](flecs::entity e, const render_component& rc, const world_transform_component& world)
			{
				if (rc.m_render_proxy != nullptr)
				{
					enqueue_render_command<"EntityUpdateTransform">(
						[world_matrix = world.m_world_matrix, proxy = rc.m_render_proxy]()
						{
							proxy->update_world_matrix(float4x4(world_matrix));
						}
					);
				}
			}
		);
	}
}
