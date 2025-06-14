/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_resource_view.h"
#include "core/core.h"


namespace i
{
	/**
	 *	The bound shader signature is an object describes all parameters bound to the shaders of all stages
	 *
  	 *	Equivalents:
 	 *		- DX: `ID3D12RootSignature`
	 *		- VK: `VkPipelineLayout`
	 *		- MT: ``
	 */
	class NENE_API gapi_bound_shader_signature : noncopyable
	{
	public:
		gapi_bound_shader_signature() = default;

		~gapi_bound_shader_signature() override = default;

		virtual uint64 hash() = 0;
	};
}
