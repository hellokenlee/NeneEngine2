/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_dynamic/gapi_cmd_context.h"

namespace i
{
	class NENE_API global_render_resource
	{
	public:
		global_render_resource();
    
		virtual ~global_render_resource() = default;

		virtual void initialize(gapi_cmd_context& context) = 0;

		static void initialize_global_render_resources(gapi_cmd_context& context);
    
	protected:
		static std::vector<global_render_resource*> s_global_render_resources; 
	};
}
