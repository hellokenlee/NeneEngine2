/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "local_space_vertex_factory.h"


namespace nene::r
{
	local_space_vertex_factory::local_space_vertex_factory(const std::vector<float3>& positions, const std::vector<float3>& normals, const std::vector<float2>& uvs)
	{
		if (!positions.empty())
		{
			auto position_vertex_stream = std::make_shared<class vertex_stream>(positions.data(), positions.size() * sizeof(std::remove_reference_t<decltype(positions)>::value_type), gapi_vertex_element_type::float3); 
			vertex_factory::add_vertex_stream("POSITION", position_vertex_stream);
		}

		if (!normals.empty())
		{
			auto normal_vertex_stream = std::make_shared<class vertex_stream>(normals.data(), normals.size() * sizeof(std::remove_reference_t<decltype(normals)>::value_type), gapi_vertex_element_type::float3); 
			vertex_factory::add_vertex_stream("NORMAL", normal_vertex_stream);
		}

		if (!uvs.empty())
		{
			auto color_vertex_stream = std::make_shared<class vertex_stream>(uvs.data(), uvs.size() * sizeof(std::remove_reference_t<decltype(uvs)>::value_type), gapi_vertex_element_type::float2); 
			vertex_factory::add_vertex_stream("TEXCOORD", color_vertex_stream);
		}
	}
}
