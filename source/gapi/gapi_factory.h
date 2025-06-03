/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/windll.h"
#include "gapi_gpu.h"
#include "gapi_swap_chain.h"

namespace i
{
	/**
	 *	The one and only graphics application interface at runtime.
	 *	This should be a singleton which can be obtain by `gapi_manager`.
	 *
	 *	Equivalents:
	 *		- DX: `IDXGIFactory`
	 *		- VK: `VkInstance`
	 *		- MT: 
	 */
	class NENE_API gapi_factory : noncopyable
	{
	public:
		gapi_factory() = default;

		~gapi_factory() override = default;
		
		virtual std::shared_ptr<gapi_gpu> create_adapter() = 0;

		virtual std::shared_ptr<gapi_swap_chain> create_swap_chain(void* hwnd, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format = gapi_pixel_format::r8g8b8a8_unorm, const uint32& multisample = 1) = 0;
	};
}
