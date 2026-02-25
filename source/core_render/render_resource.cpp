/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_resource.h"


namespace nene::r
{
	std::vector<global_render_resource*> global_render_resource::s_global_render_resources;

	render_resource::render_resource()
	{}

	global_render_resource::global_render_resource()
	{
		s_global_render_resources.emplace_back(this);
	}

	void global_render_resource::initialize_global_render_resources(gapi_cmd_context& context)
	{
		for (auto & resource : s_global_render_resources)
		{
			resource->initialize(context);
		}
	}
}
