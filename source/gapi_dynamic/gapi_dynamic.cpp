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


gapi_dynamic::gapi_dynamic(const gapi_platform& platform, void* window)
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
	m_gpu = m_factory->create_adapter();
	m_device = m_gpu->create_device();
	m_swap_chain = m_factory->create_swap_chain(window, upoint32{800, 600}, 2);
	//
	for (uint32 i = 0; i < cvar_gapi_num_context_thread.get_value_thread_unsafe(); ++i)
	{
		m_cmd_contexts.push_back(std::make_unique<gapi_cmd_context>(m_device));
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
	// TODO: Do we really need to separate `buffer` and `texture`?
	CHECK(gapi_resource_desc::is_buffer_desc(desc))
	const auto resource = m_device->create_resource(desc);
	return std::dynamic_pointer_cast<i::gapi_buffer>(resource);
}

std::shared_ptr<i::gapi_texture> gapi_dynamic::create_texture(const gapi_resource_desc& desc) const
{
	// TODO: Do we really need to separate `buffer` and `texture`?
	CHECK(gapi_resource_desc::is_texture_desc(desc));
	const auto resource = m_device->create_resource(desc);
	return std::dynamic_pointer_cast<i::gapi_texture>(resource);
}

void gapi_dynamic::start_frame()
{}

void gapi_dynamic::finish_frame()
{
	for (auto& context : m_cmd_contexts)
	{
		context->flush();
	}
}

const std::shared_ptr<i::gapi_swap_chain>& gapi_dynamic::get_swap_chain() const
{
	return m_swap_chain;
}

const std::shared_ptr<i::gapi_device>& gapi_dynamic::get_device() const
{
	return m_device;
}

std::unique_ptr<gapi_dynamic> gapi_dynamic::s_instance = {};

void gapi_dynamic::create(void* window)
{
	CHECK(window != nullptr);
	CHECK(s_instance == nullptr);
	
	const auto platform = static_cast<gapi_platform>(cvar_gapi_platform.get_value_thread_unsafe());
	s_instance = std::unique_ptr<gapi_dynamic>(new gapi_dynamic(platform, static_cast<HWND>(window)));
}

gapi_dynamic& gapi_dynamic::get()
{
	CHECKF(s_instance != nullptr, "Call `gapi_dynamic::create(...)` first for initialization.");
	return *s_instance;
}
