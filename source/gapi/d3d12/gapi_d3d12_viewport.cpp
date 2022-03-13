/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_viewport.h"

#include "d3d12_utils.h"
#include "d3d12_device.h"
#include "d3d12_cmd_list.h"
#include "d3d12_cmd_list_mgr.h"
#include "d3d12_cmd_allocator.h"


gapi_d3d12_viewport::gapi_d3d12_viewport(shared_ptr<d3d12_adapter> adapter, void* hwnd, uint32 back_buffer_num, uint32 multi_sample_num)
	: m_last_fence_value(0)
	, m_back_buffer_index(0)
	, m_fence(nullptr)
	, m_swap_chain(nullptr)
{
	//
	m_fence = shared_ptr<d3d12_fence>(new d3d12_fence(adapter->get_device(0)));

	//
	m_swap_chain = shared_ptr<d3d12_swap_chain>(new d3d12_swap_chain(adapter, hwnd, back_buffer_num, multi_sample_num));

	//
	m_back_buffer_index = m_swap_chain->get_current_back_buffer_index();
}

void gapi_d3d12_viewport::start_frame()
{
	// Wait for last submitted frame
	m_fence->wait(m_last_fence_value);

	//
	m_back_buffer_index = m_swap_chain->get_current_back_buffer_index();
}

void gapi_d3d12_viewport::finish_frame()
{
	//
	m_swap_chain->present();
	//
	m_last_fence_value = m_fence->signal();
}

shared_ptr<d3d12_texture2d> gapi_d3d12_viewport::get_back_buffer_texture()
{
	return m_swap_chain->get_back_buffer_texture(m_back_buffer_index);
}

