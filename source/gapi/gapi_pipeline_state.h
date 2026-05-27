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
	
	struct gapi_shader_parameter_index
	{
		uint8_t m_root_binding_slot = INVALID_INDEX_UINT8;
		uint8_t m_parameter_index = INVALID_INDEX_UINT8;
	};
	
	struct gapi_shader_resource_table
	{
		// map `t#` to parameter index ( within a root binding slot )
		std::vector<gapi_shader_parameter_index> m_srv_register_table;
		// map `b#` to parameter index ( within a root binding slot )
		std::vector<gapi_shader_parameter_index> m_cbv_register_table;
		// map `u#` to parameter index ( within a root binding slot )
		std::vector<gapi_shader_parameter_index> m_uav_register_table;
		// map `s#` to parameter index ( within a root binding slot )
		std::vector<gapi_shader_parameter_index> m_dynamic_sampler_register_table;
	};

	struct NENE_API gapi_shader_resource_tables
	{
		// 
		std::array<gapi_shader_resource_table, NUM_GAPI_SHADER_STAGE> m_shader_stage_register_tables;
		//
		bool m_is_finalized = false;
		std::vector<uint8_t> m_num_parameters_per_root_binding_slot;
		
		void finalize();
		
		gapi_shader_parameter_index get_parameter_index(gapi_shader_stage stage, gapi_resource_view_type stype, uint32_t reg) const;
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