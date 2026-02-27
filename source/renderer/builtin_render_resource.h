/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_render/render_resource.h"
#include "static_mesh.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_resource.h"
#include "gapi/gapi_pipeline_state_desc.h"


namespace nene::r
{
	class NENE_API builtin_static_meshes : public global_render_resource
	{
	public:
		static const builtin_static_meshes& get();
		void initialize(gapi_cmd_context& cmd_context) override;
		
		std::shared_ptr<static_mesh> m_cube;

	protected:
		builtin_static_meshes() = default;
	};
}