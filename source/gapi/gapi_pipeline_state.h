/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_shader.h"
#include "gapi_resource_view.h"

namespace  nene
{
	enum class gapi_pipeline_state_type : uint8_t
	{
		compute,
		graphics,
	};


	struct gapi_shader_resource_table
	{
		// map `t#` to pipeline parameter index
		std::vector<uint32_t> m_srv_register_table;
		// map `b#` to pipeline parameter index
		std::vector<uint32_t> m_cbv_register_table;
		// map `u#` to pipeline parameter index
		std::vector<uint32_t> m_uav_register_table;
		// map `s#` to pipeline parameter index
		std::vector<uint32_t> m_dynamic_sampler_register_table;
	};

	struct gapi_shader_resource_tables
	{
		std::array<gapi_shader_resource_table, num_gapi_shader_stage> m_shader_stage_register_tables;
		uint32_t m_num_total_parameters = 0;

		uint32_t get_parameter_index(gapi_shader_stage stage, gapi_resource_view_type stype, uint32_t reg) const
		{
			switch (stype)
			{
			case gapi_resource_view_type::constant_buffer_view:
				const auto& cbv_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_cbv_register_table;
				CHECK(cbv_table.size() > reg);
				return cbv_table[reg];
			case gapi_resource_view_type::shader_resource_view:
				const auto& srv_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_srv_register_table;
				CHECK(srv_table.size() > reg);
				return srv_table[reg];
			case gapi_resource_view_type::unordered_access_view:
				const auto& uav_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_uav_register_table;
				CHECK(uav_table.size() > reg);
				return uav_table[reg];
			case gapi_resource_view_type::texture_sampler:
				const auto& sampler_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_dynamic_sampler_register_table;
				CHECK(sampler_table.size() > reg);
				return sampler_table[reg];
			default:
				CHECK(false);
			}
			return -1;
		}
	};
	
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