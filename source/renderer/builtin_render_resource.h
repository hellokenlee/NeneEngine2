/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_render/render_resource.h"
#include "core_render/static_mesh_render_data.h"
#include "gapi_dynamic/gapi_cmd_context.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_resource.h"
#include "gapi/gapi_pipeline_state_desc.h"


namespace nene::r
{
	/** handy basic vertex data in render thread */
	class NENE_API builtin_static_mesh_render_data
	{
	public:
		static void initialize(gapi_cmd_context& cmd_context);
		static void destroy();
		static builtin_static_mesh_render_data& get();
		
		const std::shared_ptr<const static_mesh_lod_render_data>& cube() const { return m_cube.get(); }
	
	protected:
		static std::unique_ptr<builtin_static_mesh_render_data> s_instance;
		
		builtin_static_mesh_render_data(gapi_cmd_context& cmd_context);
		
		t::dual_shared_ptr<static_mesh_lod_render_data> m_cube;
	};
}