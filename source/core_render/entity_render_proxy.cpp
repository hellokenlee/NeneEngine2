/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "entity_render_proxy.h"


namespace nene::r
{
	entity_render_proxy::entity_render_proxy()
	{
		update_world_matrix(float4x4::identity());
	}

	void entity_render_proxy::update_world_matrix(const float4x4& world_matrix)
	{
		m_data.WorldMatrix = world_matrix;
		mark_constant_buffer_dirty();
	}
}
