/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_dynamic.h"

#include "gapi_pipeline_state_manager.h"
#include "core/core.h"
#include "core/file_helper.h"
#include "gapi_d3d12/gapi_d3d12_factory.h"
#include "gapi_d3d12/gapi_d3d12_shader.h"


t::console_var cvar_gapi_platform(
	"gapi.platform",
	0,
	"Select the platform that engine use:\n"
	"	0: d3d12;\n"
	"	1: vulkan;\n"
	"	2: metal;\n"
);

t::console_var cvar_shader_feature_level(
	"shader.feature_level",
	0,
	"Select the platform that engine use:\n"
	"	0: shading model 5.0;\n"
	"	1: shading model 6.0;\n"
);

static t::shared_ptr<gapi_dynamic> g_gapi_instance = {};

static constexpr int32 g_num_gapi_worker_threads = 2;


gapi_dynamic::gapi_dynamic(const gapi_platform& platform, void* window)
	: m_factory(nullptr)
	, m_adapter(nullptr)
	, m_device(nullptr)
	, m_swap_chain(nullptr)
{
	//
	switch (platform)
	{
	case gapi_platform::direct3d12:
		m_factory = t::shared_ptr<gapi_d3d12_factory>(new gapi_d3d12_factory());
		break;
	case gapi_platform::vulkan:
	case gapi_platform::metal:
		CHECK(false);
		break;
	}
	//
	m_adapter = m_factory->create_adapter();
	m_device = m_adapter->create_device();
	m_swap_chain = m_factory->create_swap_chain(window, point32{800, 600});
	//
	for (int i = 0; i < g_num_gapi_worker_threads; ++i)
	{
		m_cmd_contexts.push_back(t::make_shared<gapi_cmd_context>(m_device));
	}
}

gapi_dynamic::~gapi_dynamic()
{
	m_swap_chain.reset();
	m_device.reset();
	m_adapter.reset();
	m_factory.reset();
}

t::shared_ptr<gapi_cmd_context> gapi_dynamic::get_cmd_context(const uint32& thread_id)
{
	CHECK(thread_id < g_num_gapi_worker_threads);
	return m_cmd_contexts[thread_id];
}

t::shared_ptr<i::gapi_vertex_shader> gapi_dynamic::create_vertex_shader(const sstring& filepath, const sstring& entry) const
{
	int32 feature_level = cvar_shader_feature_level.get_value_thread_unsafe();
	const sstring source = file_helper::load_file_to_sstring(filepath);
	return m_device->create_vertex_shader(source, entry, static_cast<gapi_shader_feature_level>(feature_level), sstring("vertex_shader::") + filepath);
}

t::shared_ptr<i::gapi_pixel_shader> gapi_dynamic::create_pixel_shader(const sstring& filepath, const sstring& entry) const
{
	int32 feature_level = cvar_shader_feature_level.get_value_thread_unsafe();
	const sstring source = file_helper::load_file_to_sstring(filepath);
	return m_device->create_pixel_shader(source, entry, static_cast<gapi_shader_feature_level>(feature_level), sstring("piexl_shader::") + filepath);
}

t::shared_ptr<i::gapi_pipeline_state> gapi_dynamic::create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) const
{
	return gapi_pipeline_state_manager::get()->find_or_create_pipeline_state(desc);
}

t::shared_ptr<i::gapi_pipeline_state> gapi_dynamic::create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) const
{
	return gapi_pipeline_state_manager::get()->find_or_create_pipeline_state(desc);
}

t::shared_ptr<i::gapi_buffer> gapi_dynamic::create_buffer(const gapi_resource_desc& desc) const
{
	// TODO: Do we really need to separate `buffer` and `texture`?
	CHECK(gapi_resource_desc::is_buffer_desc(desc))
	const auto resource = m_device->create_resource(desc);
	return t::dynamic_pointer_cast<i::gapi_buffer>(resource);
}

t::shared_ptr<i::gapi_texture> gapi_dynamic::create_texture(const gapi_resource_desc& desc) const
{
	// TODO: Do we really need to separate `buffer` and `texture`?
	CHECK(gapi_resource_desc::is_texture_desc(desc));
	const auto resource = m_device->create_resource(desc);
	return t::dynamic_pointer_cast<i::gapi_texture>(resource);
}

void gapi_dynamic::start_frame()
{
}

void gapi_dynamic::finish_frame()
{
	for (const auto context : m_cmd_contexts)
	{
		context->flush();
	}
}

t::shared_ptr<i::gapi_texture> gapi_dynamic::get_back_buffer_texture() const
{
	return m_swap_chain->get_back_buffer_texture();
}

void gapi_dynamic::create(void* window)
{
	CHECK(window != nullptr);
	CHECK(g_gapi_instance == nullptr);
	
	const auto platform = static_cast<gapi_platform>(cvar_gapi_platform.get_value_thread_unsafe());
	g_gapi_instance = t::shared_ptr<gapi_dynamic>(new gapi_dynamic(platform, static_cast<HWND>(window)));
}

const t::shared_ptr<gapi_dynamic>& gapi_dynamic::get()
{
	CHECKF(g_gapi_instance != nullptr, "Call `gapi_dynamic::create(...)` first for initialization.");
	
	return g_gapi_instance;
}
