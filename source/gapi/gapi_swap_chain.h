/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource.h"


namespace i
{
	/**
	*	The swap chain is essentially a queue of images that are waiting to be presented to the screen.
	*
	*	Equivalents:
	*		- DX: ``
	*		- VK: ``
	*		- MT: ``
	*/
	class NENE_API gapi_swap_chain : noncopyable
	{
	public:
		gapi_swap_chain() = default;

		~gapi_swap_chain() override = default;

		virtual void present() = 0;

		virtual t::shared_ptr<gapi_resource> get_back_buffer_texture() = 0;

		virtual t::shared_ptr<gapi_resource> reset_back_buffer(const point32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& swap_chain_flag = 0) = 0;
	};
}