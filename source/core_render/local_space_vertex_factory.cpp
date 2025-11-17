/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "local_space_vertex_factory.h"


namespace r
{
	local_space_vertex_factory::local_space_vertex_factory(const std::vector<float3>& positions, const std::vector<float3>& normals, const std::vector<float4>& colors)
	{
		if (!positions.empty())
		{
			auto position_vertex_stream = std::make_shared<class vertex_stream>(positions.data(), positions.size() * sizeof(decltype(positions)::value_type), gapi_vertex_element_type::float3); 
			add_vertex_stream("POSITION", position_vertex_stream);
		}

		if (!normals.empty())
		{
			auto normal_vertex_stream = std::make_shared<class vertex_stream>(normals.data(), normals.size() * sizeof(decltype(normals)::value_type), gapi_vertex_element_type::float3); 
			add_vertex_stream("NORMAL", normal_vertex_stream);
		}

		if (!colors.empty())
		{
			auto color_vertex_stream = std::make_shared<class vertex_stream>(colors.data(), colors.size() * sizeof(decltype(colors)::value_type), gapi_vertex_element_type::float4); 
			add_vertex_stream("COLOR", color_vertex_stream);
		}
	}
}
