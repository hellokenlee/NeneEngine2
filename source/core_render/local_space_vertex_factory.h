/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "vertex_factory.h"


namespace nene::r
{
	/** A vertex factory which transforms vertices from local space to world space */
	class NENE_API local_space_vertex_factory : public vertex_factory
	{
	public:
		const char* get_shader_filename() const override { return "/vertex_factory/local_space_vertex_factory.h"; }
		local_space_vertex_factory(const std::vector<float3>& positions, const std::vector<float3>& normals, const std::vector<float2>& uvs);
	};
}