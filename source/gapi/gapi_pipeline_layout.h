/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_descriptor.h"
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

	/**
	 *	The array of parameters ( resources, constant, etc. ) that actually binds to a pipeline state 
	 */
	class NENE_API gapi_pipeline_parameters : noncopyable
	{
	public:
		gapi_pipeline_parameters() = default;
		
		~gapi_pipeline_parameters() override = default;

		virtual uint64 hash() = 0;

		virtual uint32 size() = 0;
	};

	/**
	 *	The array of immutable samplers that actually binds to a pipeline state
	 */
	class NENE_API gapi_immutable_samplers : noncopyable
	{
	public:
		gapi_immutable_samplers(const std::vector<gapi_sampler_desc>& sampler_descs)
			: m_sampler_descs(sampler_descs)
		{}
		
		~gapi_immutable_samplers() override = default;

		virtual uint64 hash() = 0;

		virtual uint32 size() = 0;

	protected:
		gapi_immutable_samplers() = default;
		
		std::vector<gapi_sampler_desc> m_sampler_descs;
	};
}
