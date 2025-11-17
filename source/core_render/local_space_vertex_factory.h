/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "vertex_factory.h"


namespace r
{
	/** A vertex factory which transforms vertices from local space to world space */
	class local_space_vertex_factory : public vertex_factory
	{
	public:
		const char* get_shader_filename() const override { return "/engine/shader/local_space_vertex_factory.h"; }
		local_space_vertex_factory(const std::vector<float3>& positions, const std::vector<float3>& normals, const std::vector<float4>& colors);
	};
}