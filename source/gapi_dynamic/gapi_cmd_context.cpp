/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_cmd_context.h"

extern t::console_var<uint32> cvar_gapi_num_multi_buffer;

gapi_cmd_context::gapi_cmd_context(const std::shared_ptr<i::gapi_device>& device, uint32 num_cmd_list, uint32 debug_context_id)
	: m_device(device)
	, m_debug_id(debug_context_id)
	, m_current_index(0)
	, m_previous_index(num_cmd_list - 1)
	, m_online_resource_view_cache(device)
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
	std::vector<std::shared_ptr<i::gapi_resource_view>> rtvs;
	for (auto& render_target: render_targets)
	{
		get_current_cmd_list()->transition_resource(render_target, gapi_resource_state::render_target);
		rtvs.emplace_back(render_target->get_render_target_view());
	}
	get_current_cmd_list()->set_viewports(m_viewports);
	get_current_cmd_list()->set_scissor_rects(m_scissors);
	get_current_cmd_list()->set_render_targets(rtvs, {});
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
	// Default to triangle
	set_primitive_type(gapi_primitive_type::triangle);
	//
	m_online_resource_view_cache.reset();
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

void gapi_cmd_context::set_resolution(const upoint32& resolution)
{
	m_viewports.clear();

	m_viewports.emplace_back(
		gapi_viewport_desc{
			.top_left = float2(0.0f, 0.0f),
			.resolution = float2(static_cast<float>(resolution.w), static_cast<float>(resolution.h)),
			.depth_range = float2(0.0f, 1.0f),
		}
	);

	m_scissors.clear();

	m_scissors.emplace_back(
		rect{.left = 0, .top = 0, .right = resolution.w, .bottom = resolution.h}
	);
}

void gapi_cmd_context::clear_render_target(const std::shared_ptr<i::gapi_texture>& render_target, const color::rgba<float>& clear_color) const
{
	CHECK(render_target->get_render_target_view() != nullptr);
	get_current_cmd_list()->clear_render_target_view(render_target->get_render_target_view(), clear_color);
}

void gapi_cmd_context::draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset, const uint32& instance_offset) const
{
	get_current_cmd_list()->draw(num_vertices, num_instances, vertex_offset, instance_offset);
}

void gapi_cmd_context::draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset, const uint32& vertex_offset, const uint32& instance_offset)
{
	m_online_resource_view_cache.commit_staged_resource_views();
	get_current_cmd_list()->draw_indexed(num_indices, num_instances, index_offset, vertex_offset, instance_offset);
}

void gapi_cmd_context::set_pipeline_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	//
	get_current_cmd_list()->set_pipeline_state(pipeline_state);
}

void gapi_cmd_context::set_index_buffer(const std::shared_ptr<i::gapi_buffer>& index_buffer) const
{
	get_current_cmd_list()->set_index_buffer(index_buffer);
}

void gapi_cmd_context::set_vertex_buffer(const std::shared_ptr<i::gapi_buffer>& vertex_buffer) const
{
	get_current_cmd_list()->set_vertex_buffer(vertex_buffer);
}

void gapi_cmd_context::set_primitive_type(const gapi_primitive_type& ptype) const
{
	get_current_cmd_list()->set_primitive_topology(ptype);
}

void gapi_cmd_context::bind_shader_resource_view(const gapi_shader_stage& stage, const uint32& index, const std::shared_ptr<i::gapi_resource_view>& srv)
{
	m_online_resource_view_cache.stage_resource_view();
}

void gapi_cmd_context::bind_constant_buffer_view(const gapi_shader_stage& stage, const uint32& index, const std::shared_ptr<i::gapi_resource_view>& cbv)
{
	
}

std::shared_ptr<i::gapi_resource> gapi_cmd_context::create_and_upload_resource(const gapi_resource_desc& desc, const void* initial_data)
{
	// 先创建目标的资源, 该资源不一定要 CPU 可见
	auto target_resource = m_device->create_resource(desc);
	if (desc.is_buffer())
	{
		// 创建中介资源, 该资源需要对 CPU 可见
		auto intermediate_buffer_desc = gapi_buffer_desc::create(desc.m_width, gapi_buffer_usage_flag::dynamic_buffer);
		auto intermediate_resource = m_device->create_resource(intermediate_buffer_desc);
		// 标记状态处理
		transition_resource(intermediate_resource, gapi_resource_state::copy_source);
		transition_resource(target_resource, gapi_resource_state::copy_destination);
		auto initial_data_size = desc.buffer_size();
		// 把数据从 RAM 拷贝到中介资源 VRAM 中
		intermediate_resource->map(
			[&initial_data, &initial_data_size](void* mapped)
			{
				memcpy(mapped, initial_data, initial_data_size);
			}
		);
		// 插入一个从 VRAM -> VRAM 的拷贝指令
		get_current_cmd_list()->copy_resource_region(target_resource, 0, intermediate_resource, 0, initial_data_size);
		// 延迟删除 ( 帧末删除 )
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