/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_viewport.h"
#include "gapi/gapi_template.h"

#include "d3d12/d3d12_fence.h"
#include "d3d12/d3d12_adapter.h"
#include "d3d12/d3d12_texture.h"
#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_swap_chain.h"


class gapi_d3d12_viewport : public t::impl<gapi_d3d12_viewport, gapi_viewport>
{
public:
	gapi_d3d12_viewport(t::shared_ptr<d3d12_adapter> adapter, HWND hwnd, uint32 back_buffer_num, uint32 multi_sample_num);
	~gapi_d3d12_viewport() override = default;

public:
	void start_frame();

	void finish_frame();

	t::shared_ptr<d3d12_texture2d> get_back_buffer_texture();

public:
	//
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissor_rect;
	//
	uint64 m_last_fence_value;
	uint32 m_back_buffer_index;
	//
	t::shared_ptr<d3d12_fence> m_fence;
	t::shared_ptr<d3d12_swap_chain> m_swap_chain;
};
