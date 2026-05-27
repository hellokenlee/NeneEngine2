/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state.h"

namespace nene
{
	void gapi_shader_resource_tables::finalize()
	{
		// generate the num parameters of each root binding slot
		CHECK(!m_is_finalized);
		for (const auto& table : m_shader_stage_register_tables)
		{
			std::array register_table_ptrs = {&table.m_cbv_register_table, &table.m_srv_register_table, &table.m_uav_register_table, &table.m_dynamic_sampler_register_table};
			for (const auto table_ptr : register_table_ptrs)
			{
				for (const auto& index : *table_ptr)
				{
					if (m_num_parameters_per_root_binding_slot.size() <= index.m_root_binding_slot)
					{
						m_num_parameters_per_root_binding_slot.resize(index.m_root_binding_slot + 1);
					}
					if (m_num_parameters_per_root_binding_slot[index.m_root_binding_slot] < index.m_parameter_index + 1)
					{
						m_num_parameters_per_root_binding_slot[index.m_root_binding_slot] = index.m_parameter_index + 1;
					}
				}
			}
		}
		m_is_finalized = true;
	}

	gapi_shader_parameter_index gapi_shader_resource_tables::get_parameter_index(gapi_shader_stage stage, gapi_resource_view_type stype, uint32_t reg) const
	{
		CHECK(m_is_finalized);
		switch (stype)
		{
		case gapi_resource_view_type::constant_buffer_view:
			{
				const auto& cbv_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_cbv_register_table;
				CHECK(cbv_table.size() > reg);
				return cbv_table[reg];
			}
		case gapi_resource_view_type::shader_resource_view:
			{
				const auto& srv_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_srv_register_table;
				CHECK(srv_table.size() > reg);
				return srv_table[reg];
			}
		case gapi_resource_view_type::unordered_access_view:
			{
				const auto& uav_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_uav_register_table;
				CHECK(uav_table.size() > reg);
				return uav_table[reg];
			}
		case gapi_resource_view_type::texture_sampler:
			{
				const auto& sampler_table = m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_dynamic_sampler_register_table;
				CHECK(sampler_table.size() > reg);
				return sampler_table[reg];
			}
		default:
			CHECK(false);
		}
		return {};
	}
}
