/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


namespace nene
{
	/**
	 *	The command list layout is for specify the information for indirect draw
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12CommandSignature`
	 *		- VK: ``
	 *		- MT: ``
	 */
	class gapi_cmd_layout : noncopyable
	{
	public:
		gapi_cmd_layout() = default;
		~gapi_cmd_layout() override = default;
	};
}
