/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_viewport.h"

#include "d3d12/d3d12_utils.h"
#include "d3d12/d3d12_device.h"
#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_cmd_list_mgr.h"
#include "d3d12/d3d12_cmd_allocator.h"


gapi_d3d12_viewport::gapi_d3d12_viewport(const rect& area)
	: super()
{
	gapi_d3d12_viewport::set_rect(area);
}

void gapi_d3d12_viewport::set_rect(const rect& area)
{
	const uint16 width = area.right - area.left;
	const uint16 height = area.bottom - area.top;
	m_viewport = CD3DX12_VIEWPORT {0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)};
	m_scissor_rect = CD3DX12_RECT {0, 0, width, height};
}

rect gapi_d3d12_viewport::make_rect_from_hwnd(HWND hwnd)
{
	RECT win {0, 0, 0, 0};
	if (!GetWindowRect(hwnd, &win))
	{
		LOG(d3d12, warning, TXT("Failed to get rect area from window handler!"));
	}
	return rect {static_cast<uint32>(win.left), static_cast<uint32>(win.top), static_cast<uint32>(win.right), static_cast<uint32>(win.bottom)};
}
