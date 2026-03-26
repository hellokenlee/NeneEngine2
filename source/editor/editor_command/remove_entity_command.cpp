/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "remove_entity_command.h"
#include "engine/engine_loop.h"
#include "engine/world.h"


namespace nene
{
	extern logger editor_;
	
	remove_entity_command::remove_entity_command(const uint64_t& eid)
		: m_eid(eid)
	{
	}
	
	void remove_entity_command::execute()
	{
		const auto& w = engine_loop::get_world();
		if (w == nullptr)
		{
			log(editor_, error, "failed to remove entity {}: world is null", m_eid);
			return;
		}
		
		if (!w->remove_entity(m_eid))
		{
			log(editor_, error, "failed to remove entity {}", m_eid);
		}
	}
}
