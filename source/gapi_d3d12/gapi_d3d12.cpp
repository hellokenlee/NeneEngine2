/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12.h"
#include "gapi_d3d12_shader.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_viewport.h"
#include "gapi_d3d12_pipeline_state.h"

#include "d3d12/d3d12_globals.h"
#include "d3d12/d3d12_adapter.h"
#include "d3d12/d3d12_shader.h"

#include <windows.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>


gapi_d3d12::gapi_d3d12(HWND hwnd)
	: super()
	, m_device(nullptr)
	, m_adapter(nullptr)
	, m_viewport(nullptr)
	, m_swapchain(nullptr)
{
	//
	m_adapter = d3d12_adapter::select_adapter();

	//
	m_device = t::make_shared<d3d12_device>(m_adapter);
	m_device->init();

	//
	m_viewport = t::make_shared<gapi_d3d12_viewport>(gapi_d3d12_viewport::make_rect_from_hwnd(hwnd));
	m_swapchain = t::make_shared<gapi_d3d12_swapchain>(m_adapter, hwnd, g_d3d12_back_buffer_count, g_d3d12_back_buffer_multisample_count);
	
	// Init non-default contexts
	constexpr uint32 worker_thread_num = 1;
	for (uint32 idx = 0; idx < worker_thread_num; ++idx)
	{
		m_contexts.emplace_back(
			t::make_shared<gapi_d3d12_cmd_context>(m_device)
		);
	}
}

gapi_d3d12::gapi_d3d12(ID3D12Device* device)
	: super()
	, m_device(nullptr)
	, m_adapter(nullptr)
	, m_viewport(nullptr)
	, m_swapchain(nullptr)
{
	// Get existing device
	const LUID luid = device->GetAdapterLuid();
	m_adapter = d3d12_adapter::select_adapter(luid);
	m_device = t::make_shared<d3d12_device>(m_adapter, device);
	m_device->init();

	// We dont need swapchain 
	m_viewport = t::make_shared<gapi_d3d12_viewport>(rect{});

	// Init non-default contexts
	constexpr uint32 worker_thread_num = 1;
	for (uint32 idx = 0; idx < worker_thread_num; ++idx)
	{
		m_contexts.emplace_back(
			t::make_shared<gapi_d3d12_cmd_context>(m_device)
		);
	}
}

gapi_d3d12::~gapi_d3d12()
{
	//
	for (uint32 idx = 0; idx < m_contexts.size(); ++idx)
	{
		m_contexts[idx]->flush(true);
	}

	//
	m_swapchain->finish_frame();
	m_swapchain.reset();

	//
	m_viewport.reset();

	//
	m_contexts.clear();

	//
	m_device->clear();
	m_device.reset();

	//
	m_adapter->remove_all_devices();
	m_adapter.reset();

	//
	if (g_d3d12_debug)
	{
		WinComPtr<IDXGIDebug1> debug_com;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug_com))))
		{
			debug_com->ReportLiveObjects(DXGI_DEBUG_ALL, static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}
	}
}

t::shared_ptr<gapi_cmd_context> gapi_d3d12::get_cmd_context(const int32 id)
{
	CHECK(id < m_contexts.size());

	return m_contexts[id];
}

void gapi_d3d12::start_frame()
{
	if (m_swapchain != nullptr)
	{
		m_swapchain->start_frame();
	}
}

void gapi_d3d12::finish_frame()
{
	if (m_swapchain != nullptr)
	{
		m_swapchain->finish_frame();
	}
}

t::shared_ptr<gapi_vertex_shader> gapi_d3d12::create_vertex_shader(const gapi_shader_initializer& initializer)
{
	return t::make_shared<gapi_d3d12_vertex_shader>(initializer.get_shader_source(), initializer.get_shader_entry(), initializer.get_shader_file());
}

t::shared_ptr<gapi_pixel_shader> gapi_d3d12::create_pixel_shader(const gapi_shader_initializer& initializer)
{
	return t::make_shared<gapi_d3d12_pixel_shader>(initializer.get_shader_source(), initializer.get_shader_entry(), initializer.get_shader_file());
}

t::shared_ptr<gapi_compute_pipeline_state> gapi_d3d12::create_compute_pipeline_state(const gapi_compute_pipeline_state_initializer&)
{
	return nullptr;	
}

t::shared_ptr<gapi_graphics_pipeline_state> gapi_d3d12::create_graphic_pipeline_state(const gapi_graphics_pipeline_state_initializer& initializer)
{
	return t::make_shared<gapi_d3d12_graphics_pipeline_state>(m_device, initializer);
}

t::shared_ptr<gapi_vertex_buffer> gapi_d3d12::create_vertex_buffer(const size_t& buffer_stride, const size_t& buffer_size, const gapi_resource_usage& buffer_usage)
{
	return t::make_shared<gapi_d3d12_vertex_buffer>(m_device, buffer_stride, buffer_size);
}

void* gapi_d3d12::lock_vertex_buffer(t::shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	const t::shared_ptr<gapi_d3d12_vertex_buffer> buffer = gapi_d3d12_vertex_buffer::cast(vertex_buffer);
	return buffer->get_d3d12_vertex_buffer()->map();
}

void gapi_d3d12::unlock_vertex_buffer(t::shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	// Unmap the buffer
	const t::shared_ptr<gapi_d3d12_vertex_buffer> buffer = gapi_d3d12_vertex_buffer::cast(vertex_buffer);
	buffer->get_d3d12_vertex_buffer()->unmap();

	// Fence and wait for buffer uploading
	const auto buffer_fence = t::make_shared<d3d12_fence>(m_device);
	const uint64 fence_value = buffer_fence->signal();
	buffer_fence->wait(fence_value);
}

t::shared_ptr<gapi_texture> gapi_d3d12::create_texture_2d(const gapi_texture_desc& desc)
{
	return t::make_shared<gapi_d3d12_texture_2d>(m_device, desc);
}
