/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "vertex_factory.h"
#include "shader_map.h"

#include <string>
#include <unordered_set>


namespace nene::r
{
	class NENE_API material_shader_map : public shader_map
	{
	public:
		material_shader_map(const std::string& vertex_shader_source);
		
		void add_vertex_factory(const vertex_factory& vf);
		
	protected:
		std::vector<std::string> m_vertex_shader_sources;
		std::unordered_set<std::string> m_supported_vertex_factories;
	};
}
