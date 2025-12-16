/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_shader.h"


enum class gapi_pipeline_state_type : uint8
{
	compute,
	graphics,
};


struct gapi_shader_resource_table
{
	// map `t#` to pipeline parameter index
	std::vector<uint32> m_srv_register_table;
	// map `b#` to pipeline parameter index
	std::vector<uint32> m_cbv_register_table;
	// map `u#` to pipeline parameter index
	std::vector<uint32> m_uav_register_table;
	// map `s#` to pipeline parameter index
	std::vector<uint32> m_dynamic_sampler_register_table;
};
using gapi_shader_resource_tables = std::array<gapi_shader_resource_table, num_gapi_shader_stage>;

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
		gapi_pipeline_state(const gapi_shader_resource_tables& shader_resource_tables)
			: m_shader_resource_tables(shader_resource_tables)
		{}

		~gapi_pipeline_state() override = default;

		virtual bool is_compute() const = 0;
		virtual bool is_graphics() const = 0;
		
		const gapi_shader_resource_tables& get_shader_resource_tables() const { return m_shader_resource_tables; }

	protected:
		gapi_shader_resource_tables m_shader_resource_tables;
	};
}