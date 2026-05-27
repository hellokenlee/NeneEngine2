/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_online_resource_view_cache.h"
#include "gapi_invalid_resources.h"


namespace nene
{
	gapi_online_resource_view_cache::gapi_online_resource_view_cache(const std::shared_ptr<gapi_device>& device)
	{
		// we only cache srv, cbv, usv online
		m_online_resource_view_allocator = device->create_resource_view_allocator(gapi_resource_view_type::shader_resource_view, gapi_resource_view_allocator_type::online);
	}

	void gapi_online_resource_view_cache::reset()
	{
		m_online_resource_view_allocator->reset();
	}

	void gapi_online_resource_view_cache::initialize(const gapi_shader_resource_tables& shader_resource_tables)
	{
		m_shader_resource_tables = shader_resource_tables;
		m_staged_resource_views.clear();
		m_staged_resource_views.resize(m_shader_resource_tables.m_num_parameters_per_root_binding_slot.size());
		for (auto root_binding_slot = 0;  root_binding_slot < m_shader_resource_tables.m_num_parameters_per_root_binding_slot.size(); ++root_binding_slot)
		{
			m_staged_resource_views[root_binding_slot].resize(m_shader_resource_tables.m_num_parameters_per_root_binding_slot[root_binding_slot]);
		}
	}

	void gapi_online_resource_view_cache::stage_resource_view(gapi_shader_stage stage, uint32_t reg, const std::shared_ptr<gapi_resource_view>& view)
	{
		//
		const auto vtype = view->get_type();
		CHECK(view->get_type() == gapi_resource_view_type::constant_buffer_view || view->get_type() == gapi_resource_view_type::shader_resource_view || view->get_type() == gapi_resource_view_type::unordered_access_view);
		//
		auto parameter_index = m_shader_resource_tables.get_parameter_index(stage, vtype, reg);
		CHECK(m_staged_resource_views.size() > parameter_index.m_root_binding_slot)
		CHECK(m_staged_resource_views[parameter_index.m_root_binding_slot].size() > parameter_index.m_parameter_index)
		//
		m_staged_resource_views[parameter_index.m_root_binding_slot][parameter_index.m_parameter_index] = view;
	}

	void gapi_online_resource_view_cache::commit_staged_resource_views(const std::shared_ptr<gapi_cmd_list>& cmd_list, const std::shared_ptr<gapi_device>& device)
	{
		// fill empty parameter with invalid resource view
		for (const auto& table : m_shader_resource_tables.m_shader_stage_register_tables)
		{
			for (const auto& index : table.m_srv_register_table)
			{
				if (m_staged_resource_views[index.m_root_binding_slot][index.m_parameter_index] == nullptr)
				{
					m_staged_resource_views[index.m_root_binding_slot][index.m_parameter_index] = gapi_invalid_resources::get().get_invalid_texture()->get_shader_resource_view();
				}
			}
		}
		
		// TODO: 减少拷贝, 只拷贝变化的
		// TODO: 检查绑定的数量和初始化的数量是否一致
		for (size_t parameter_index = 0; parameter_index < m_staged_resource_views.size(); parameter_index++)
		{
			// the resource views shared same type ( cbv, srv ... ) and same stage
			const auto& offline_resource_views = m_staged_resource_views[parameter_index];
			// copy them to online heap
			std::shared_ptr<gapi_resource_view> base_online_resource_view = nullptr; 
			for (const auto& offline_resource_view : offline_resource_views)
			{
				auto online_resource_view = m_online_resource_view_allocator->allocate_resource_view(offline_resource_view->get_type());
				device->copy_resource_view(online_resource_view, offline_resource_view);
				// mark the base one
				if (base_online_resource_view == nullptr)
				{
					base_online_resource_view = online_resource_view;
				}
			}
			CHECK(base_online_resource_view != nullptr);
			//
			cmd_list->bind_ranged_resource_views(static_cast<uint32_t>(parameter_index), base_online_resource_view, m_online_resource_view_allocator);
		}
		//
		
	}
}
