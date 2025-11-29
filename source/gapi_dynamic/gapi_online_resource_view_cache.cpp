/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_online_resource_view_cache.h"

gapi_online_resource_view_cache::gapi_online_resource_view_cache(const std::shared_ptr<i::gapi_device>& device)
{
	// TODO: Paged allocator
	// we only cache srv, cbv, usv online
	m_online_resource_view_allocator = device->create_resource_view_allocator(gapi_resource_view_type::shader_resource_view, gapi_resource_view_allocator_type::online, 1024);
}

void gapi_online_resource_view_cache::reset()
{
	// 
}

void gapi_online_resource_view_cache::initialize()
{
}

void gapi_online_resource_view_cache::stage_resource_view()
{
	
}

void gapi_online_resource_view_cache::commit_staged_resource_views()
{
	
}
