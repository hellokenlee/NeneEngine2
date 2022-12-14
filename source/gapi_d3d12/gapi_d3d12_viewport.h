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
	gapi_d3d12_viewport(const rect& area);
	~gapi_d3d12_viewport() override = default;

	virtual void set_rect(const rect& area) override;
	
	[[nodiscard]] const D3D12_VIEWPORT& get_d3d_viewport() const { return m_viewport; }
	[[nodiscard]] const D3D12_RECT& get_d3d_scissor_rect() const { return m_scissor_rect; }

	static rect make_rect_from_hwnd(HWND hwnd);
public:
	//
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissor_rect;
};
