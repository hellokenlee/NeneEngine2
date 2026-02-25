/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource.h"


namespace nene
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
		
		virtual size_t num_back_buffers() const = 0;

		virtual uint2 get_back_buffer_size() = 0;
		
		virtual const std::shared_ptr<gapi_texture>& get_back_buffer() const = 0;
		
		virtual const std::shared_ptr<gapi_texture>& get_back_buffer(size_t index) const = 0;
		
		virtual uint32_t get_current_back_buffer_index() = 0;

		virtual void resize_back_buffer(const uint2& resolution) = 0;

		virtual void set_debug_name(const std::string& debug_name) {};
	};
}