/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_cmd_list.h"
#include "gapi_device.h"

namespace i
{
	gapi_cmd_list::gapi_cmd_list(const std::shared_ptr<gapi_device>& device)
		: m_device(device)
	{}

	void gapi_cmd_list::track_resource(const std::shared_ptr<gapi_resource>& resource)
	{
		m_tracked_resources.push_back(resource);
	}

	void gapi_cmd_list::release_tracked_resources()
	{
		m_tracked_resources.clear();
	}

	std::shared_ptr<gapi_resource> gapi_cmd_list::create_and_upload_resource(const gapi_resource_desc& desc, const void* initial_data)
	{
		//
		auto target_resource = m_device->create_resource(desc);
		if (desc.is_buffer())
		{
			//
			auto intermediate_buffer_desc = gapi_buffer_desc::create(desc.m_width, gapi_buffer_usage_flag::dynamic_buffer);
			auto intermediate_resource = m_device->create_resource(intermediate_buffer_desc);
			//
			transition_resource(intermediate_resource, gapi_resource_state::copy_source);
			transition_resource(target_resource, gapi_resource_state::copy_destination);
			auto initial_data_size = desc.buffer_size();
			//
			intermediate_resource->map(
				[&initial_data, &initial_data_size](void* mapped)
				{
					memcpy(mapped, initial_data, initial_data_size);
				}
			);
			copy_resource_region(target_resource, 0, intermediate_resource, 0, initial_data_size);
			//
			track_resource(intermediate_resource);
		}
		else if (desc.is_texture())
		{
			NOT_IMPLEMENTED();
		}
		else
		{
			CHECK(false);
		}
		//
		return target_resource;
	}

	void gapi_cmd_list::reset(const std::shared_ptr<gapi_cmd_allocator>& allocator, const std::shared_ptr<gapi_pipeline_state>& pipeline_state)
	{
		release_tracked_resources();
	}
}
