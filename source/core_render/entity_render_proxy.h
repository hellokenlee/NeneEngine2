/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "render_constant_buffer.h"
#include "shader/cppshared/object_data.hlsli"


namespace nene::r
{
	/**
	 *	the render thread proxy of an entity
	 */
	class NENE_API entity_render_proxy : public render_constant_buffer<SObjectData>
	{
	public:
		entity_render_proxy();
		~entity_render_proxy() override = default;
		
		void update_world_matrix(const float4x4& world_matrix);
	};
}