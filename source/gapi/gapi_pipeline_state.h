/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_pipeline_state_type
{
	compute,
	graphics,
};

namespace i
{
	/**
	*	The pipeline state object that binds into command list.
	*
	*	Equivalents:
	*		- DX: `ID3D12PipelineState`
	*		- VK: `VkPipeline`
	*		- MT: ``
	*/
	class NENE_API gapi_pipeline_state : noncopyable
	{
	public:
		gapi_pipeline_state() = default;

		~gapi_pipeline_state() override = default;

		virtual bool is_compute() const = 0;
		
		virtual bool is_graphics() const = 0;
	};
}