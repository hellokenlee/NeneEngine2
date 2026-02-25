/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "d3d12_utils.h"


namespace nene
{
	class NENE_API gapi_d3d12_factory : public gapi_factory
	{
	public:
		gapi_d3d12_factory();
		~gapi_d3d12_factory() override = default;

		static int32_t get_d3d12_version();
		
		std::shared_ptr<gapi_gpu> create_gpu() override;

		std::shared_ptr<gapi_swap_chain> create_swap_chain(void* hwnd, const std::shared_ptr<gapi_cmd_queue>& cmd_queue, const uint2& resolution, uint32_t multibuffer, const gapi_pixel_format& pixel_format, uint32_t multisample) override;
		
	protected:
		WinComPtr<IDXGIFactory2> m_factory2;
		WinComPtr<IDXGIFactory4> m_factory4;
		WinComPtr<IDXGIFactory7> m_factory7;
	};
}