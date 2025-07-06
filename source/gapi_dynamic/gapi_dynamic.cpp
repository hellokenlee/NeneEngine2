/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_dynamic.h"

#include "gapi_pipeline_state_manager.h"
#include "core/core.h"
#include "core/file_helper.h"
#include "gapi_d3d12/gapi_d3d12_factory.h"
#include "gapi_d3d12/gapi_d3d12_shader.h"

t::console_var<uint32> cvar_gapi_platform("gapi.platform", 0u, "The graphics api that engine use: 0: d3d12; 1: vulkan; 2:metal;");
t::console_var<uint32> cvar_gapi_num_context_thread("gapi.num_context_thread", 1u, "How many cmd contexts to create;", console_var_flag::read_only);
t::console_var<uint32> cvar_gapi_shader_feature_level("gapi.shader.feature_level", 0u, "Select the platform that engine use: 0: shading model 5.0;1: shading model 6.0;");
t::console_var<uint32> cvar_gapi_num_swap_chain_buffer("gapi.num_swap_chain_buffer", 2u, "How many buffers should create in swap-chain; Default is 2 for double buffering.", console_var_flag::read_only);

gapi_dynamic::gapi_dynamic(const gapi_platform& platform, void* window, const upoint32& window_size)
	: m_factory(nullptr)
	, m_gpu(nullptr)
	, m_device(nullptr)
	, m_swap_chain(nullptr)
{
	//
	switch (platform)
	{
	case gapi_platform::direct3d12:
		m_factory = std::make_unique<gapi_d3d12_factory>();
		break;
	case gapi_platform::vulkan:
	case gapi_platform::metal:
		NOT_IMPLEMENTED();
		break;
	}
	//
	const auto num_multi_buffer = cvar_gapi_num_swap_chain_buffer.value();
	// TODO: multi gpu support
	m_gpu = m_factory->create_gpu();
	m_device = m_gpu->create_device();
	m_swap_chain = m_factory->create_swap_chain(window, m_device->get_cmd_queue(gapi_cmd_type::graphics), window_size, num_multi_buffer);
	//
	for (uint32 context_id = 0; context_id < cvar_gapi_num_context_thread.get_value_thread_unsafe(); ++context_id)
	{
		m_cmd_contexts.push_back(std::make_unique<gapi_cmd_context>(m_device, num_multi_buffer, context_id));
	}
	//
	for (auto& fence_values : m_cmd_queue_fence_values)
	{
		fence_values.resize(num_multi_buffer, 0);
	}
}

gapi_dynamic::~gapi_dynamic() = default;

gapi_cmd_context& gapi_dynamic::get_cmd_context(const uint32& context_id) const
{
	CHECK(context_id < cvar_gapi_num_context_thread.get_value_thread_unsafe());
	return *m_cmd_contexts[context_id];
}

std::shared_ptr<i::gapi_pipeline_state> gapi_dynamic::create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) const
{
	return m_device->create_compute_pipeline_state(desc);
}

std::shared_ptr<i::gapi_pipeline_state> gapi_dynamic::create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) const
{
	return m_device->create_graphics_pipeline_state(desc);
}

std::shared_ptr<i::gapi_buffer> gapi_dynamic::create_buffer(const gapi_resource_desc& desc) const
{
	CHECK(desc.is_buffer())
	const auto resource = m_device->create_resource(desc);
	return std::dynamic_pointer_cast<i::gapi_buffer>(resource);
}

std::shared_ptr<i::gapi_texture> gapi_dynamic::create_texture(const gapi_resource_desc& desc) const
{
	CHECK(desc.is_texture());
	const auto resource = m_device->create_resource(desc);
	return std::dynamic_pointer_cast<i::gapi_texture>(resource);
}

void gapi_dynamic::flush() const
{
	m_device->get_cmd_queue(gapi_cmd_type::graphics)->flush();
	m_device->get_cmd_queue(gapi_cmd_type::compute)->flush();
	m_device->get_cmd_queue(gapi_cmd_type::copy)->flush();
}

void gapi_dynamic::start_frame() const
{
	for (auto& context : m_cmd_contexts)
	{
		context->reset();
	}
}

void gapi_dynamic::finish_frame() const
{
	// TODO: early execution and flush 
	for (auto& context : m_cmd_contexts)
	{
		// TODO: multi command queue supports
		context->close();
		// the `gapi_cmd_context::close()` would make current to previous
		m_device->get_cmd_queue(gapi_cmd_type::graphics)->execute_cmd_list(context->get_previous_cmd_list());
	}
}

void gapi_dynamic::present_frame()
{
	// TODO: multi command queue supports
	
	// mark current frame fence value
	const auto current_frame_index = m_swap_chain->get_current_back_buffer_index();
	const auto current_frame_fence_value = m_device->get_cmd_queue(gapi_cmd_type::graphics)->signal();
	m_cmd_queue_fence_values[static_cast<uint32>(gapi_cmd_type::graphics)][current_frame_index] = current_frame_fence_value;

	// present and move to next frame
	m_swap_chain->present();

	// wait for previous frame's fence
	const auto previous_frame_index = m_swap_chain->get_current_back_buffer_index();
	const auto previous_frame_fence_value = m_cmd_queue_fence_values[static_cast<uint32>(gapi_cmd_type::graphics)][previous_frame_index];
	m_device->get_cmd_queue(gapi_cmd_type::graphics)->wait_for_fence_value(previous_frame_fence_value);
}

const std::shared_ptr<i::gapi_swap_chain>& gapi_dynamic::get_swap_chain() const
{
	return m_swap_chain;
}

void gapi_dynamic::resize_swap_chain(const upoint32& new_size)
{
	if (m_swap_chain->get_back_buffer_size() != new_size)
	{
		flush();
		// invalidate in-flight command lists:
		// since all previous frame's fence value must smaller than current's.
		// reset all fence values to current's to ensure that the fence value is at least the value that was last signaled on the command queue.
		const auto current_frame_index = m_swap_chain->get_current_back_buffer_index();
		for (auto& fence_value : m_cmd_queue_fence_values[static_cast<uint32>(gapi_cmd_type::graphics)])
		{
			fence_value = m_cmd_queue_fence_values[static_cast<uint32>(gapi_cmd_type::graphics)][current_frame_index];
		}
		//
		m_swap_chain->resize_back_buffer(new_size);
	}
}

const std::shared_ptr<i::gapi_device>& gapi_dynamic::get_device() const
{
	return m_device;
}

std::unique_ptr<gapi_dynamic> gapi_dynamic::s_instance = {};

void gapi_dynamic::create(void* window, const upoint32& window_size)
{
	CHECK(window != nullptr);
	CHECK(s_instance == nullptr);
	
	const auto platform = static_cast<gapi_platform>(cvar_gapi_platform.get_value_thread_unsafe());
	s_instance = std::unique_ptr<gapi_dynamic>(new gapi_dynamic(platform, static_cast<HWND>(window), window_size));
}

gapi_dynamic& gapi_dynamic::get()
{
	CHECKF(s_instance != nullptr, "Call `gapi_dynamic::create(...)` first for initialization.");
	return *s_instance;
}
