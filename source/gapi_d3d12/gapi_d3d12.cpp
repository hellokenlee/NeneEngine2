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


gapi_d3d12::gapi_d3d12(void* hwnd)
	: gapi()
	, m_device(nullptr)
	, m_adapter(nullptr)
	, m_viewport(nullptr)
{
	//
	m_adapter = d3d12_adapter::select_adapter();

	//
	m_device = shared_ptr<d3d12_device>(new d3d12_device(m_adapter));
	m_device->init();

	//
	m_viewport = shared_ptr<gapi_d3d12_viewport>(new gapi_d3d12_viewport(m_adapter, hwnd, g_d3d12_back_buffer_count, g_d3d12_back_buffer_multisample_count));
}

gapi_d3d12::~gapi_d3d12()
{
	//
	m_viewport->finish_frame();
	m_viewport.reset();
	m_viewport = nullptr;

	//
	m_device->clear();
	m_device.reset();
	m_device = nullptr;

	//
	m_adapter->remove_all_devices();
	m_adapter.reset();
	m_adapter = nullptr;

	//
	if (g_d3d12_debug)
	{
		WinComPtr<IDXGIDebug1> debug_com;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug_com))))
		{
			debug_com->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}
	}
}

void gapi_d3d12::start_frame()
{
	m_viewport->start_frame();
}

void gapi_d3d12::finish_frame()
{
	m_viewport->finish_frame();
}

shared_ptr<gapi_vertex_shader> gapi_d3d12::create_vertex_shader(const gapi_shader_initializer& initializer)
{
	shared_ptr<gapi_d3d12_vertex_shader> result(
		new gapi_d3d12_vertex_shader(initializer.m_shader_source, initializer.m_shader_entry, initializer.m_shader_file)
	);

	result->compile();
	return result;
}

shared_ptr<gapi_pixel_shader> gapi_d3d12::create_pixel_shader(const gapi_shader_initializer& initializer)
{
	shared_ptr<gapi_d3d12_pixel_shader> result(
		new gapi_d3d12_pixel_shader(initializer.m_shader_source, initializer.m_shader_entry, initializer.m_shader_file)
	);
	result->compile();
	return result;
}

shared_ptr<gapi_compute_pipeline_state> gapi_d3d12::create_compute_pipeline_state(const gapi_compute_pipeline_state_initializer&)
{
	return nullptr;	
}

shared_ptr<gapi_graphics_pipeline_state> gapi_d3d12::create_graphic_pipeline_state(const gapi_graphics_pipeline_state_initializer& initializer)
{
	shared_ptr<gapi_d3d12_graphics_pipeline_state> result(new gapi_d3d12_graphics_pipeline_state(m_device, initializer));
	return result;
}

shared_ptr<gapi_vertex_buffer> gapi_d3d12::create_vertex_buffer(const size_t& buffer_stride, const size_t& buffer_size, const gapi_resource_usage& buffer_usage)
{
	shared_ptr<gapi_d3d12_vertex_buffer> result(new gapi_d3d12_vertex_buffer(m_device, buffer_stride, buffer_size));
	return result;
}

void* gapi_d3d12::lock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	shared_ptr<gapi_d3d12_vertex_buffer> buffer = gapi_d3d12_vertex_buffer::cast(vertex_buffer);
	return buffer->map();
}

void gapi_d3d12::unlock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	// Unmap the buffer
	shared_ptr<gapi_d3d12_vertex_buffer> buffer = gapi_d3d12_vertex_buffer::cast(vertex_buffer);
	buffer->unmap();

	// Fence and wait for buffer uploading
	auto buffer_fence = shared_ptr<d3d12_fence>(new d3d12_fence(m_device));
	uint64 fence_value = buffer_fence->signal();
	buffer_fence->wait(fence_value);
}
