/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_list.h"


enum class gapi_cmd_type : uint8_t
{
	graphics = 0,
	compute,
	copy,
};

namespace nene
{
	/**
	 *	The command queue within a device.
	 *	A command list got submit and execute in a command queue.
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12CommandQueue`
	 *		- VK: `VkQueue`
	 *		- MT: `MTLCommandQueue`
	 */
	class NENE_API gapi_cmd_queue : noncopyable
	{
	public:
		gapi_cmd_queue(gapi_cmd_type cmd_type)
			: m_cmd_type(cmd_type)
		{}

		~gapi_cmd_queue() override = default;

		virtual void flush() = 0;
		
		virtual uint64_t signal() = 0;

		virtual void wait_for_fence_value(uint64_t fence_value) = 0;
		
		virtual void execute_cmd_list(const std::shared_ptr<gapi_cmd_list>& cmd_list) = 0;

		virtual gapi_cmd_type get_cmd_type() const { return m_cmd_type; }
		
	protected:
		gapi_cmd_type m_cmd_type;
	};
}