/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_inspector.h"
#include "engine/engine.h"
#include "engine/world.h"


namespace nene
{
	entity_inspector& entity_inspector::get()
	{
		static entity_inspector instance;
		return instance;
	}

	void entity_inspector::inspect(uint64_t eid)
	{
		m_inspecting_eid = eid;
		notify(entity_inspect_event());
	}

	void entity_inspector::modified() const
	{
		if (m_inspecting_eid != 0)
		{
			flecs::entity e = engine::get().get_world()->get_ecs().entity(static_cast<flecs::entity_t>(m_inspecting_eid));
			e.each(
			[&e](flecs::id id)
				{
					if (id.is_entity())
					{
						e.modified(id);
					}
				}
			);
		}
	}

	std::vector<g::reflection::variant> entity_inspector::get_inspecting_components() const
	{
		std::vector<g::reflection::variant> components;
		
		flecs::entity e = engine::get().get_world()->get_ecs().entity(static_cast<flecs::entity_t>(m_inspecting_eid));
		e.each(
			[&e, &components](flecs::id id)
			{
				if (id.is_entity())
				{
					auto cid = id.entity().id();
					void* comp = e.get_mut(cid);
					auto cname = id.entity().name();
					g::reflection::variant py_object = g::reflection::component(cid, comp);
					if (!py_object.is_none())
					{
						components.emplace_back(py_object);
					}
				}
			}
		);
		
		return components;
	}
}
