/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace i
{
	/**
	 *	The abstraction of a single GPU hardware ( or GPU adapter ).
	 *	We can have multiple `gapi_hardware` in PC, but only one in console or mobile.
	 *
	 *	Equivalents:
	 *		- DX: `IDXGIAdapter`
	 *		- VK:
	 *		- MT: 
	 */
	class NENE_API gapi_harward
	{
	public:
		gapi_harward() = default;

		virtual ~gapi_harward() = default;

		gapi_harward(gapi_harward& other) = delete;

		gapi_harward& operator=(const gapi_harward& other) = delete;
	};
}