/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "material_shader_map.h"
#include "gapi/gapi_resource.h"

#include <unordered_map>


namespace nene::r
{
	class NENE_API render_material
	{
	public:
		render_material();
		
		const material_shader_map& get_shader_map(const vertex_factory& vf);
		
		const std::unordered_map<uint32_t, std::shared_ptr<gapi_resource>>& get_resource_bindings(gapi_shader_stage stage) const;
		
	protected:
		material_shader_map m_shader_map;
		std::unordered_map<uint32_t, std::shared_ptr<gapi_resource>> m_resource_bindings;
	};
}
