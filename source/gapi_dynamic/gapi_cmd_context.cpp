/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_cmd_context.h"


namespace nene
{
	extern t::console_var<uint32_t> cvar_gapi_num_multi_buffer;

	gapi_cmd_context::gapi_cmd_context(const std::shared_ptr<gapi_device>& device, uint32_t num_cmd_list, uint32_t debug_context_id)
		: m_device(device)
		, m_debug_id(debug_context_id)
		, m_current_index(0)
		, m_previous_index(num_cmd_list - 1)
		, m_online_resource_view_cache(device)
	{
		for (uint32_t i = 0; i < num_cmd_list; i++)
		{
			one_frame_context_data context_data;
			context_data.m_cmd_allocator = m_device->create_cmd_allocator(gapi_cmd_type::graphics);
			context_data.m_cmd_list = m_device->create_cmd_list(gapi_cmd_type::graphics, context_data.m_cmd_allocator);
			context_data.m_cmd_list->set_debug_name(std::format(L"Context#{}::CommandList#{}", m_debug_id, i));
			m_frame_contexts.emplace_back(std::move(context_data));
		}
	}

	void gapi_cmd_context::begin_render_pass(const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil) const
	{
		std::vector<std::shared_ptr<gapi_resource_view>> rtvs;
		for (auto& render_target: render_targets)
		{
			get_current_cmd_list()->transition_resource(render_target, gapi_resource_state::render_target);
			rtvs.emplace_back(render_target->get_render_target_view());
		}
		get_current_cmd_list()->set_viewports(m_viewports);
		get_current_cmd_list()->set_scissor_rects(m_scissors);
		get_current_cmd_list()->set_render_targets(rtvs, depth_stencil ? depth_stencil->get_depth_stencil_view() : nullptr);
	}

	void gapi_cmd_context::end_render_pass() const
	{
		
	}

	scoped_render_pass gapi_cmd_context::render_pass(const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil)
	{
		scoped_render_pass render_pass(this, render_targets, depth_stencil);
		return render_pass;
	}

	void gapi_cmd_context::reset()
	{
		//
		get_current_cmd_allocator()->reset();
		get_current_cmd_list()->reset(get_current_cmd_allocator(), nullptr);
		//
		release_deferred_resources();
		// Default to triangle
		set_primitive_type(gapi_primitive_type::triangle);
		//
		m_online_resource_view_cache.reset();
	}

	const std::shared_ptr<gapi_cmd_list>& gapi_cmd_context::close()
	{
		//
		get_current_cmd_list()->close();
		//
		m_previous_index = m_current_index;
		m_current_index = (m_current_index + 1) % m_frame_contexts.size();
		// return the closed command list
		return get_previous_cmd_list();
	}

	void gapi_cmd_context::set_resolution(const uint2& resolution)
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
			rect32_t{.left = 0, .top = 0, .right = resolution.w, .bottom = resolution.h}
		);
	}

	void gapi_cmd_context::clear_render_target(const std::shared_ptr<gapi_texture>& render_target, const color::rgba<float>& clear_color) const
	{
		CHECK(render_target->get_render_target_view() != nullptr);
		get_current_cmd_list()->clear_render_target_view(render_target->get_render_target_view(), clear_color);
	}

	void gapi_cmd_context::draw(uint32_t num_vertices, uint32_t num_instances, uint32_t vertex_offset, uint32_t instance_offset)
	{
		m_online_resource_view_cache.commit_staged_resource_views(get_current_cmd_list(), m_device);
		get_current_cmd_list()->draw(num_vertices, num_instances, vertex_offset, instance_offset);
	}

	void gapi_cmd_context::draw_indexed(uint32_t num_indices, uint32_t num_instances, uint32_t index_offset, uint32_t vertex_offset, uint32_t instance_offset)
	{
		m_online_resource_view_cache.commit_staged_resource_views(get_current_cmd_list(), m_device);
		get_current_cmd_list()->draw_indexed(num_indices, num_instances, index_offset, vertex_offset, instance_offset);
	}

	void gapi_cmd_context::set_pipeline_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state)
	{
		//
		m_online_resource_view_cache.initialize(pipeline_state->get_shader_resource_tables());
		//
		get_current_cmd_list()->set_pipeline_state(pipeline_state);
	}

	void gapi_cmd_context::set_index_buffer(const std::shared_ptr<gapi_buffer>& index_buffer) const
	{
		get_current_cmd_list()->set_index_buffer(index_buffer);
	}

	void gapi_cmd_context::set_vertex_buffer(const std::shared_ptr<gapi_buffer>& vertex_buffer) const
	{
		get_current_cmd_list()->set_vertex_buffer(vertex_buffer);
	}

	void gapi_cmd_context::set_primitive_type(const gapi_primitive_type& ptype) const
	{
		get_current_cmd_list()->set_primitive_topology(ptype);
	}

	void gapi_cmd_context::bind_shader_resource(const gapi_shader_stage& stage, uint32_t reg, const std::shared_ptr<gapi_resource>& resource)
	{
		CHECK(resource->get_shader_resource_view() != nullptr);
		// TODO: auto barrier resolve
		m_online_resource_view_cache.stage_resource_view(stage, reg, resource->get_shader_resource_view());
	}

	void gapi_cmd_context::bind_constant_buffer(const gapi_shader_stage& stage, uint32_t reg, const std::shared_ptr<gapi_buffer>& buffer)
	{
		//
		CHECK(buffer->get_constant_buffer_view() != nullptr);
		// TODO: auto barrier resolve
		// TODO: inline root descriptor handling
		m_online_resource_view_cache.stage_resource_view(stage, reg, buffer->get_constant_buffer_view());
	}

	std::shared_ptr<gapi_buffer> gapi_cmd_context::create_and_upload_buffer(const gapi_resource_desc& desc, const void* initial_data)
	{
		//
		CHECK(desc.is_buffer());
		// 先创建目标的资源
		auto target_buffer = std::dynamic_pointer_cast<gapi_buffer>(m_device->create_resource(desc));
		if (t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::dynamic_buffer))
		{
			// 对于 CPU 可见直接拷贝
			auto initial_data_size = desc.buffer_size();
			target_buffer->map(
				[&initial_data, &initial_data_size](void* mapped)
				{
					memcpy(mapped, initial_data, initial_data_size);
				}
			);
		}
		else
		{
			// 对于 CPU 不可见, 创建中介资源, 该资源需要对 CPU 可见
			auto intermediate_buffer_desc = gapi_buffer_desc::create(desc.m_width, gapi_buffer_usage_flag::dynamic_buffer);
			auto intermediate_buffer = std::dynamic_pointer_cast<gapi_buffer>(m_device->create_resource(intermediate_buffer_desc));
			// 标记状态处理
			transition_resource(intermediate_buffer, gapi_resource_state::copy_source);
			transition_resource(target_buffer, gapi_resource_state::copy_destination);
			auto initial_data_size = desc.buffer_size();
			// 把数据从 RAM 拷贝到中介资源 VRAM 中
			intermediate_buffer->map(
				[&initial_data, &initial_data_size](void* mapped)
				{
					memcpy(mapped, initial_data, initial_data_size);
				}
			);
			// 插入一个从 VRAM -> VRAM 的拷贝指令
			get_current_cmd_list()->copy_buffer_region(target_buffer, 0, intermediate_buffer, 0, initial_data_size);
			// 延迟删除 ( 帧末删除 )
			deferred_release(intermediate_buffer);
		}
		return target_buffer;
	}

	std::shared_ptr<gapi_texture> gapi_cmd_context::create_and_upload_texture(const gapi_resource_desc& desc, const std::vector<const void*>& initial_data)
	{
		CHECK(desc.is_texture());
		auto target_texture = std::dynamic_pointer_cast<gapi_texture>(m_device->create_resource(desc));
		if (t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::cpu_writable))
		{
			// 对于 CPU 可见直接拷贝
			NOT_IMPLEMENTED();
		}
		else
		{
			//
			uint64_t intermediate_buffer_required_size = 0;
			auto intermediate_buffer_sublayouts = m_device->calculate_buffer_layout(target_texture, 0, static_cast<uint32_t>(initial_data.size()), intermediate_buffer_required_size);
			// 对于 CPU 不可见, 创建中介资源, 该资源需要对 CPU 可见
			auto intermediate_buffer_desc = gapi_buffer_desc::create(intermediate_buffer_required_size, gapi_buffer_usage_flag::dynamic_buffer);
			auto intermediate_buffer = std::dynamic_pointer_cast<gapi_buffer>(m_device->create_resource(intermediate_buffer_desc));
			// 标记状态处理
			transition_resource(intermediate_buffer, gapi_resource_state::copy_source);
			transition_resource(target_texture, gapi_resource_state::copy_destination);
			// 把数据从 RAM 拷贝到中介资源 VRAM 中
			intermediate_buffer->map(
				[&initial_data, &intermediate_buffer_sublayouts](void* immediate_buffer_mapped)
				{
					// 按逐个 subtexture 顺序填充 intermediate buffer 数据
					for (size_t subindex = 0; subindex < intermediate_buffer_sublayouts.size(); ++subindex)
					{
						//
						const auto& sublayout = intermediate_buffer_sublayouts[subindex];
						uint8_t* dst_start = static_cast<uint8_t*>(immediate_buffer_mapped) + sublayout->offset();
						const uint8_t* src_start = static_cast<const uint8_t*>(initial_data[subindex]);
						// 
						for (uint32_t z = 0; z < sublayout->num_slices(); ++z)
						{
							// 一个 slice 一共有 num_rows * bytes_per_row 个字节
							uint8_t* dst_slice_start = dst_start + z * sublayout->num_rows() * sublayout->padded_bytes_per_row();
							const uint8_t* src_slice_start = src_start + z * sublayout->num_rows() * sublayout->unpadded_bytes_per_row();
							//
							for (uint32_t y = 0; y < sublayout->num_rows(); ++y)
							{
								// 逐行拷贝
								// NOTE: 这里假设 `initial_data` 里面的一行像素的字节长度和在 vram 里面的是一样长的
								memcpy(dst_slice_start + y * sublayout->padded_bytes_per_row(), src_slice_start + y * sublayout->unpadded_bytes_per_row(), sublayout->padded_bytes_per_row());
							}
						}
					}
				}
			);
			// 逐个 subtexture 插入从 VRAM -> VRAM 的拷贝指令
			for (size_t subindex = 0; subindex < initial_data.size(); ++subindex)
			{
				get_current_cmd_list()->copy_buffer_region(target_texture, static_cast<uint32_t>(subindex), intermediate_buffer, intermediate_buffer_sublayouts[subindex]);
			}
			// 延迟删除 ( 帧末删除 )
			deferred_release(intermediate_buffer);
		}
		return target_texture;
	}

	void gapi_cmd_context::transition_resource(const std::shared_ptr<gapi_resource>& resource, const gapi_resource_state& to_state) const
	{
		// TODO: from state check and skip
		get_current_cmd_list()->transition_resource(resource, to_state);
	}

	void gapi_cmd_context::deferred_release(const std::shared_ptr<gapi_resource>& resource)
	{
		m_frame_contexts[m_current_index].m_tracked_resources.emplace_back(resource);
	}

	void gapi_cmd_context::release_deferred_resources()
	{
		m_frame_contexts[m_current_index].m_tracked_resources.clear();
	}

	scoped_render_pass::scoped_render_pass(gapi_cmd_context* context, const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil)
		: m_context(context)
	{
		m_context->begin_render_pass(render_targets, depth_stencil);
	}

	scoped_render_pass::~scoped_render_pass()
	{
		m_context->end_render_pass();
	}
}