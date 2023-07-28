/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_device.h"


namespace i
{
	/**
	 *	The abstraction of a single GPU hardware ( or GPU adapter ).
	 *	We can have multiple `gapi_hardware` in PC, but only one in console or mobile.
	 *
	 *	Equivalents:
	 *		- DX: `IDXGIAdapter`
	 *		- VK: `VkPhysicalDevice`
	 *		- MT: 
	 */
	class NENE_API gapi_adapter : noncopyable
	{
	public:
		gapi_adapter() = default;

		~gapi_adapter() override = default;

		virtual t::shared_ptr<gapi_device> create_device() = 0;

		virtual uint32 get_device_num() = 0;
	
		virtual t::shared_ptr<gapi_device> get_device(const uint32& index) = 0;
	};
}