/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_fence.h"
#include "d3d12_adapter.h"
#include "d3d12_texture2d.h"
#include "d3d12_cmd_list.h"


class d3d12_viewport : public noncopyable, public d3d12_adapter_child
{
public:
	d3d12_viewport(shared_ptr<d3d12_adapter> adapter, void* hwnd, uint32 back_buffer_num, uint32 multi_sample_num);

	void begin_frame();

	void end_frame();

	void present();

protected:
	HWND m_hwnd;
	WinComPtr<IDXGISwapChain3> m_swap_chain;

protected:
	uint64 m_last_fence_value;
	shared_ptr<d3d12_fence> m_fence;

	shared_ptr<d3d12_cmd_list> m_cmd_list;

	uint32 m_back_buffer_num;
	uint32 m_multi_sample_num;
	uint32 m_back_buffer_index;
	vector<shared_ptr<d3d12_texture2d>> m_back_buffers;
};
