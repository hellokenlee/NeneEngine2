/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include "vertex_factory.h"
#include "shader_map.h"

namespace r
{
	class NENE_API material_shader_map : public shader_map
	{
	public:
		material_shader_map(const std::string& vertex_shader_source, const vertex_factory& vf);
	};
}
