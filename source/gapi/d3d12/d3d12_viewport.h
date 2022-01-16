/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_adapter.h"
#include "d3d12_texture2d.h"

class d3d12_viewport : public noncopyable, public d3d12_adapter_child
{
public:
	d3d12_viewport(shared_ptr<d3d12_adapter> adapter, void* hwnd, uint32 back_buffer_num, uint32 multi_sample_num);

	bool present(bool vsync=false);

protected:
	HWND m_hwnd;
	WinComPtr<IDXGISwapChain3> m_swap_chain;

protected:
	uint32 m_back_buffer_num;
	uint32 m_multi_sample_num;
	uint32 m_back_buffer_index;

	vector<shared_ptr<d3d12_texture2d>> m_back_buffers;
};
