/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_resource.h"


namespace i
{
	std::vector<render_resource*> render_resource::s_global_render_resources;

	render_resource::render_resource()
	{
		s_global_render_resources.emplace_back(this);
	}

	void render_resource::initialize_global_render_resources(gapi_cmd_context& context)
	{
		for (auto & resource : s_global_render_resources)
		{
			resource->initialize(context);
		}
	}
}
