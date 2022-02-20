/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_gapi.h"
#include "d3d12_globals.h"
#include "d3d12_adapter.h"
#include "d3d12_shader.h"

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>


d3d12_gapi::d3d12_gapi(void* hwnd)
{
	//
	m_adapter = d3d12_adapter::select_adapter();

	//
	m_device = shared_ptr<d3d12_device>(new d3d12_device(m_adapter));
	m_device->init();

	//
	m_viewport = shared_ptr<d3d12_viewport>(new d3d12_viewport(m_adapter, hwnd, g_d3d12_back_buffer_count, g_d3d12_back_buffer_multisample_count));


	//
	auto ps = shared_ptr<d3d12_pixel_shader>(new d3d12_pixel_shader(TEXT("shader/simple.hlsl"), TEXT("PSMain")));
	auto vs = shared_ptr<d3d12_vertex_shader>(new d3d12_vertex_shader(TEXT("shader/simple.hlsl"), TEXT("VSMain")));
	ps->compile();
	vs->compile();
}

d3d12_gapi::~d3d12_gapi()
{
	//
	m_viewport->end_frame();
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

void d3d12_gapi::begin_drawing_viewport()
{
	m_viewport->begin_frame();
}

void d3d12_gapi::end_drawing_viewport()
{
	m_viewport->present();
	m_viewport->end_frame();
}

shared_ptr<gapi_cmd_context> d3d12_gapi::create_cmd_context()
{
	CHECK(0);
	return nullptr;
}