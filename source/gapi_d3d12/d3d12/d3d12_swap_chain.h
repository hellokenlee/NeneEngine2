/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_adapter.h"
#include "d3d12_texture.h"


class d3d12_swap_chain : public noncopyable, public d3d12_adapter_child
{
public:
	d3d12_swap_chain(t::shared_ptr<d3d12_adapter> adapter, HWND hwnd, uint32 back_buffer_num, uint32 multi_sample_num);

public:
	void present();

	uint32 get_current_back_buffer_index();

	t::shared_ptr<d3d12_texture_2d> get_back_buffer_texture(uint32 index) { return m_back_buffers[index]; }

protected:
	//
	HWND m_hwnd;

	//
	uint32 m_back_buffer_num;
	uint32 m_multi_sample_num;

	//
	WinComPtr<IDXGISwapChain3> m_swap_chain;

	//
	t::dynamic_array<t::shared_ptr<d3d12_texture_2d>> m_back_buffers;
};