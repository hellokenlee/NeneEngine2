/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_observer.h"

#include "component/render_component.h"
#include "core_render/render_thread.h"
#include "engine/component/static_mesh_component.h"
#include "core_render/static_mesh_render_proxy.h"
#include "asset/asset_registry.h"


namespace nene::g
{
	render_observer::render_observer(const world& w)
	{
		w.get_ecs().observer<static_mesh_component>()
			.event(flecs::OnSet)
			.each(
				[&w](flecs::entity e, static_mesh_component& sm)
				{
					if (e.has(flecs::Prefab))
					{
						return;
					}
					if (sm.m_asset.is_valid())
					{
						//
						auto asset = asset_registry::get().load(sm.m_asset);
						const auto& render_data = asset->get_or_create_render_data();
						auto proxy = std::make_shared<r::static_mesh_render_proxy>(render_data);
						e.set<render_component>({ proxy });
						//
						CHECK(w.get_render_scene() != nullptr);
						enqueue_render_command<"AddRenderProxy">(
							[proxy = std::move(proxy), scene = w.get_render_scene()]()
							{
								scene->add_render_proxy(proxy);
							}
						);
					}
				}
			)
		;
		
		w.get_ecs().observer<render_component>()
			.event(flecs::OnRemove)
			.each(
				[](flecs::entity e, render_component& c) 
				{
					if (c.m_render_proxy != nullptr)
					{
						enqueue_render_command<"RemoveRenderProxy">(
							[proxy = std::move(c.m_render_proxy)]() mutable
							{
								proxy.reset();
							}
						);
					}
				}
			)
		;
	}
}
