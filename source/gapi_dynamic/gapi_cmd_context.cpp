/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_cmd_context.h"

extern t::console_var<uint32> cvar_gapi_num_multi_buffer;

gapi_cmd_context::gapi_cmd_context(const std::shared_ptr<i::gapi_device>& device, uint32 num_cmd_list, uint32 debug_context_id)
	: m_device(device)
	, m_debug_id(debug_context_id)
	, m_current_index(0)
	, m_previous_index(num_cmd_list - 1)
{
	for (uint32 i = 0; i < num_cmd_list; i++)
	{
		one_frame_context_data context_data;
		context_data.m_cmd_allocator = m_device->create_cmd_allocator(gapi_cmd_type::graphics);
		context_data.m_cmd_list = m_device->create_cmd_list(gapi_cmd_type::graphics, context_data.m_cmd_allocator);
		context_data.m_cmd_list->set_debug_name(std::format(L"Context#{}::CommandList#{}", m_debug_id, i));
		m_frame_contexts.emplace_back(std::move(context_data));
	}
}

void gapi_cmd_context::begin_render_pass(const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets) const
{
	for (auto& render_target: render_targets)
	{
		get_current_cmd_list()->transition_resource(render_target, gapi_resource_state::render_target); 
	}
}

void gapi_cmd_context::end_render_pass() const
{
	
}

scoped_render_pass gapi_cmd_context::render_pass(const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets)
{
	scoped_render_pass render_pass(this, render_targets);
	return render_pass;
}

void gapi_cmd_context::reset()
{
	//
	get_current_cmd_allocator()->reset();
	get_current_cmd_list()->reset(get_current_cmd_allocator(), nullptr);
	//
	release_tracked_resources();
}

const std::shared_ptr<i::gapi_cmd_list>& gapi_cmd_context::close()
{
	//
	get_current_cmd_list()->close();
	//
	m_previous_index = m_current_index;
	m_current_index = (m_current_index + 1) % m_frame_contexts.size();
	// return the closed command list
	return get_previous_cmd_list();
}

void gapi_cmd_context::clear_render_target(const std::shared_ptr<i::gapi_texture>& render_target, const color::rgba<float>& clear_color) const
{
	CHECK(render_target->get_render_target_view() != nullptr);
	get_current_cmd_list()->clear_render_target_view(render_target->get_render_target_view(), clear_color);
}

std::shared_ptr<i::gapi_resource> gapi_cmd_context::create_and_upload_resource(const gapi_resource_desc& desc, const void* initial_data)
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
		get_current_cmd_list()->copy_resource_region(target_resource, 0, intermediate_resource, 0, initial_data_size);
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
//

void gapi_cmd_context::transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& to_state) const
{
	// TODO: from state check and skip
	get_current_cmd_list()->transition_resource(resource, to_state);
}

void gapi_cmd_context::track_resource(const std::shared_ptr<i::gapi_resource>& resource)
{
	m_frame_contexts[m_current_index].m_tracked_resources.emplace_back(resource);
}

void gapi_cmd_context::release_tracked_resources()
{
	m_frame_contexts[m_current_index].m_tracked_resources.clear();
}

scoped_render_pass::scoped_render_pass(gapi_cmd_context* context, const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets)
	: m_context(context)
{
	m_context->begin_render_pass(render_targets);
}

scoped_render_pass::~scoped_render_pass()
{
	m_context->end_render_pass();
}