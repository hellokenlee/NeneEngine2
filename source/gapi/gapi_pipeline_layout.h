/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


namespace i
{
	/**
	*	The pipeline layout is the only thing can bind into command list
	*
	*	Equivalents:
	*		- DX: `ID3D12RootSignature`
	*		- VK: `VkPipelineLayout`
	*		- MT: ``
	*/
	class NENE_API gapi_pipeline_layout : noncopyable
	{
	public:
		gapi_pipeline_layout() = default;

		~gapi_pipeline_layout() override = default;

		virtual uint64 hash() = 0;
	};

	class NENE_API gapi_pipeline_parameter : noncopyable
	{
	public:
		gapi_pipeline_parameter() = default;
		
		~gapi_pipeline_parameter() override = default;

		virtual uint64 hash() = 0;
	};
}
