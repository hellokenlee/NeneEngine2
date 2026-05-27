/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_dynamic.h"
#include "gapi_pipeline_state_manager.h"

#include "core/core.h"
#include "gapi_d3d12/gapi_d3d12_factory.h"
#include "gapi_d3d12/gapi_d3d12_shader.h"

namespace nene
{
	t::console_var<uint32_t> cvar_gapi_platform("gapi.platform", 0u, "The graphics api that engine use: 0: d3d12; 1: vulkan; 2:metal;");
	t::console_var<uint32_t> cvar_gapi_num_context_thread("gapi.num_context_thread", 1u, "How many cmd contexts to create;", console_var_flag::read_only);
	t::console_var<uint32_t> cvar_gapi_shader_feature_level("gapi.shader.feature_level", 0u, "Select the platform that engine use: 0: shading model 5.0;1: shading model 6.0;");
	t::console_var<uint32_t> cvar_gapi_num_buffering("gapi.num_buffering", 2u, "The num of N-Buffering; Default is 2 for double buffering.", console_var_flag::read_only);

	gapi_dynamic::gapi_dynamic(const gapi_platform& platform, void* window, const uint2& window_size)
		: m_factory(nullptr)
		, m_gpu(nullptr)
		, m_device(nullptr)
		, m_swap_chain(nullptr)
	{
		//
		switch (platform)
		{
		case gapi_platform::d3d12:
			m_factory = std::make_unique<gapi_d3d12_factory>();
			break;
		case gapi_platform::vulkan:
		case gapi_platform::metal:
			NOT_IMPLEMENTED();
			break;
		}
		//
		const auto num_back_buffer = cvar_gapi_num_buffering.value();
		// TODO: multi gpu support
		m_gpu = m_factory->create_gpu();
		m_device = m_gpu->create_device();
		// TODO: Expandable Allocator ( e.g. paged allocator )
		m_rtv_allocator = m_device->create_resource_view_allocator(gapi_resource_view_type::render_target_view, gapi_resource_view_allocator_type::offline);
		m_dsv_allocator = m_device->create_resource_view_allocator(gapi_resource_view_type::depth_stencil_view, gapi_resource_view_allocator_type::offline);
		m_sampler_allocator = m_device->create_resource_view_allocator(gapi_resource_view_type::texture_sampler, gapi_resource_view_allocator_type::offline);
		m_cbv_srv_uav_allocator = m_device->create_resource_view_allocator(gapi_resource_view_type::shader_resource_view, gapi_resource_view_allocator_type::offline);
		//
		m_swap_chain = m_factory->create_swap_chain(window, m_device->get_cmd_queue(gapi_cmd_type::graphics), window_size, num_back_buffer);
		//
		for (uint32_t context_id = 0; context_id < cvar_gapi_num_context_thread.get_value_thread_unsafe(); ++context_id)
		{
			m_cmd_contexts.push_back(std::make_unique<gapi_cmd_context>(m_device, num_back_buffer, context_id));
		}
		
		// 
		for (size_t index = 0; index < m_swap_chain->num_back_buffers(); ++index)
		{
			create_texture_views(m_swap_chain->get_back_buffer(index));
		}
		for (auto& context : m_cmd_contexts)
		{
			context->set_resolution(window_size);
			context->reset(m_swap_chain->get_current_back_buffer_index());
		}
		//
		for (auto& fence_values : m_cmd_queue_fence_values)
		{
			fence_values.resize(num_back_buffer, 0);
		}
	}

	gapi_dynamic::~gapi_dynamic() = default;

	gapi_cmd_context& gapi_dynamic::get_cmd_context(uint32_t context_id) const
	{
		CHECK(context_id < cvar_gapi_num_context_thread.get_value_thread_unsafe());
		return *m_cmd_contexts[context_id];
	}

	std::shared_ptr<gapi_pipeline_state> gapi_dynamic::create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) const
	{
		return m_device->create_compute_pipeline_state(desc);
	}

	std::shared_ptr<gapi_pipeline_state> gapi_dynamic::create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) const
	{
		return m_device->create_graphics_pipeline_state(desc);
	}

	void gapi_dynamic::create_buffer_views(const std::shared_ptr<gapi_buffer>& buffer) const
	{
		// create offline resource views
		const auto& create_flag = buffer->get_resource_desc().m_buffer_usage_flag;
		if (t::has_flag(create_flag, gapi_buffer_usage_flag::constant_buffer))
		{
			auto cbv = m_cbv_srv_uav_allocator->allocate_resource_view(gapi_resource_view_type::constant_buffer_view);
			m_device->create_constant_buffer_view(cbv, buffer);
			buffer->set_constant_buffer_view(cbv);
		}
		if (t::has_flag(create_flag, gapi_buffer_usage_flag::shader_resource))
		{
			auto srv = m_cbv_srv_uav_allocator->allocate_resource_view(gapi_resource_view_type::constant_buffer_view);
			m_device->create_constant_buffer_view(srv, buffer);
			buffer->set_shader_resource_view(srv);
		}
		if (t::has_flag(create_flag, gapi_buffer_usage_flag::unordered_access))
		{
			auto uav = m_cbv_srv_uav_allocator->allocate_resource_view(gapi_resource_view_type::unordered_access_view);
			m_device->create_unordered_access_view(uav, buffer);
			buffer->set_unordered_access_view(uav);
		}
	}

	std::shared_ptr<gapi_buffer> gapi_dynamic::create_buffer(const gapi_resource_desc& desc) const
	{
		CHECK(desc.is_buffer())
		auto resource = m_device->create_resource(desc);
		auto buffer = std::dynamic_pointer_cast<gapi_buffer>(resource);
		create_buffer_views(buffer);
		return buffer;
	}

	void gapi_dynamic::create_texture_views(const std::shared_ptr<gapi_texture>& texture) const
	{
		// create offline resource views
		const auto& create_flag = texture->get_resource_desc().m_texture_create_flag;
		if (t::has_flag(create_flag, gapi_texture_create_flag::as_render_target))
		{
			auto rtv = m_rtv_allocator->allocate_resource_view(gapi_resource_view_type::render_target_view);
			m_device->create_render_target_view(rtv, texture);
			texture->set_render_target_view(rtv);
		}
		if (t::has_flag(create_flag, gapi_texture_create_flag::as_depth_stencil))
		{
			auto dsv = m_dsv_allocator->allocate_resource_view(gapi_resource_view_type::depth_stencil_view);
			m_device->create_depth_stencil_view(dsv, texture);
			texture->set_depth_stencil_view(dsv);
		}
		if (t::has_flag(create_flag, gapi_texture_create_flag::as_shader_resource))
		{
			auto srv = m_cbv_srv_uav_allocator->allocate_resource_view(gapi_resource_view_type::shader_resource_view);
			m_device->create_shader_resource_view(srv, texture);
			texture->set_shader_resource_view(srv);
		}
		if (t::has_flag(create_flag, gapi_texture_create_flag::as_unordered_access))
		{
			auto uav = m_cbv_srv_uav_allocator->allocate_resource_view(gapi_resource_view_type::unordered_access_view);
			m_device->create_unordered_access_view(uav, texture);
			texture->set_unordered_access_view(uav);
		}
	}

	std::shared_ptr<gapi_texture> gapi_dynamic::create_texture(const gapi_resource_desc& desc) const
	{
		CHECK(desc.is_texture());
		auto resource = m_device->create_resource(desc);
		auto texture = std::dynamic_pointer_cast<gapi_texture>(resource);
		create_texture_views(texture);
		return texture;
	}

	void gapi_dynamic::flush() const
	{
		m_device->get_cmd_queue(gapi_cmd_type::graphics)->flush();
		m_device->get_cmd_queue(gapi_cmd_type::compute)->flush();
		m_device->get_cmd_queue(gapi_cmd_type::copy)->flush();
	}

	void gapi_dynamic::present_frame()
	{
		// TODO: multi command queue supports
		
		// execute current frame commands
		for (auto& context : m_cmd_contexts)
		{
			const auto& current_frame_cmd_list = context->close();
			m_device->get_cmd_queue(gapi_cmd_type::graphics)->execute_cmd_list(current_frame_cmd_list);
		}
		
		// mark current frame fence value
		const auto current_frame_index = m_swap_chain->get_current_back_buffer_index();
		const auto current_frame_fence_value = m_device->get_cmd_queue(gapi_cmd_type::graphics)->signal();
		m_cmd_queue_fence_values[static_cast<uint32_t>(gapi_cmd_type::graphics)][current_frame_index] = current_frame_fence_value;

		// present and move the swapchain to the next frame
		m_swap_chain->present();
		
		// wait for the next frame's last cycle fence
		const auto next_frame_index = m_swap_chain->get_current_back_buffer_index();
		const auto next_frame_last_cycle_fence_value = m_cmd_queue_fence_values[static_cast<uint32_t>(gapi_cmd_type::graphics)][next_frame_index];
		m_device->get_cmd_queue(gapi_cmd_type::graphics)->wait_for_fence_value(next_frame_last_cycle_fence_value);
		
		// reset the context for the next frame's commands
		for (auto& context : m_cmd_contexts)
		{
			context->reset(next_frame_index);
		}
	}

	const std::shared_ptr<gapi_swap_chain>& gapi_dynamic::get_swap_chain() const
	{
		return m_swap_chain;
	}

	void gapi_dynamic::resize_swap_chain(const uint2& new_size)
	{
		if (m_swap_chain->get_back_buffer_size() != new_size)
		{
			// execute recorded commands
			for (auto& context : m_cmd_contexts)
			{
				const auto& current_frame_cmd_list = context->close();
				m_device->get_cmd_queue(gapi_cmd_type::graphics)->execute_cmd_list(current_frame_cmd_list);
			}
			// wait for execution
			flush();
			
			// do resize the back buffer
			m_swap_chain->resize_back_buffer(new_size);
			for (size_t index = 0; index < m_swap_chain->num_back_buffers(); ++index)
			{
				create_texture_views(m_swap_chain->get_back_buffer(index));
			}
			for (auto& context : m_cmd_contexts)
			{
				context->set_resolution(new_size);
			}

			// align the timeline of every frame's fence value ( as if they are all 0 at the beginning, now they are all set to current frame's value ) 
			const auto current_frame_index = m_swap_chain->get_current_back_buffer_index();
			for (auto& context : m_cmd_contexts)
			{
				context->reset(current_frame_index);
			}
			const auto current_frame_fence_value = m_cmd_queue_fence_values[static_cast<uint32_t>(gapi_cmd_type::graphics)][current_frame_index];
			for (auto& fence_value : m_cmd_queue_fence_values[static_cast<uint32_t>(gapi_cmd_type::graphics)])
			{
				fence_value = current_frame_fence_value;
			}
		}
	}

	const std::shared_ptr<gapi_device>& gapi_dynamic::get_device() const
	{
		return m_device;
	}

	std::unique_ptr<gapi_dynamic> gapi_dynamic::s_instance = {};

	gapi_dynamic& gapi_dynamic::initialize(void* window, const uint2& window_size)
	{
		CHECK(window != nullptr);
		CHECK(s_instance == nullptr);
		
		const auto platform = static_cast<gapi_platform>(cvar_gapi_platform.get_value_thread_unsafe());
		s_instance = std::unique_ptr<gapi_dynamic>(new gapi_dynamic(platform, static_cast<HWND>(window), window_size));
		//
		return *s_instance;
	}

	gapi_dynamic& gapi_dynamic::get()
	{
		CHECK_HINTS(s_instance != nullptr, "Call `gapi_dynamic::initialize(...)` first for initialization.");
		return *s_instance;
	}
}
