/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_gapi.h"
#include "d3d12_adapter.h"
#include "d3d12_globals.h"


d3d12_gapi::d3d12_gapi(void* hwnd)
{
	shared_ptr<d3d12_adapter> adapter = d3d12_adapter::select_adapter();

	//
	m_device = shared_ptr<d3d12_device>(new d3d12_device(adapter));
	m_device->init();

	//
	m_viewport = shared_ptr<d3d12_viewport>(new d3d12_viewport(adapter, hwnd, g_d3d12_back_buffer_count, g_d3d12_back_buffer_multisample_count));
}

d3d12_gapi::~d3d12_gapi()
{
	shared_ptr<d3d12_adapter> adapter = m_device->get_parent_adapter();

	m_device.reset();
	m_device = nullptr;

	adapter.reset();
	adapter = nullptr;
}
