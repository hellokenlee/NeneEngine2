/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_online_resource_view_cache.h"

gapi_online_resource_view_cache::gapi_online_resource_view_cache(const std::shared_ptr<i::gapi_device>& device)
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
	m_staged_resource_views.resize(m_shader_resource_tables.m_num_total_parameters);
}

void gapi_online_resource_view_cache::stage_resource_view(gapi_shader_stage stage, uint32 reg, const std::shared_ptr<i::gapi_resource_view>& view)
{
	//
	const auto vtype = view->get_type();
	CHECK(view->get_type() == gapi_resource_view_type::constant_buffer_view || view->get_type() == gapi_resource_view_type::shader_resource_view || view->get_type() == gapi_resource_view_type::unordered_access_view);
	//
	uint32_t parameter_index = m_shader_resource_tables.get_parameter_index(stage, vtype, reg);
	m_staged_resource_views[parameter_index] = view;
}

void gapi_online_resource_view_cache::commit_staged_resource_views(const std::shared_ptr<i::gapi_cmd_list>& cmd_list, const std::shared_ptr<i::gapi_device>& device)
{
	// TODO: 减少拷贝, 只拷贝变化的
	for (size_t parameter_index = 0; parameter_index < m_staged_resource_views.size(); parameter_index++)
	{
		//
		const auto& offline_resource_view = m_staged_resource_views[parameter_index];
		auto online_resource_view = m_online_resource_view_allocator->allocate_resource_view(offline_resource_view->get_type());
		//
		device->copy_resource_view(online_resource_view, offline_resource_view);
		//
		cmd_list->bind_ranged_resource_views(static_cast<uint32_t>(parameter_index), online_resource_view, m_online_resource_view_allocator);
	}
}
