/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_device.h"
#include "gapi/gapi_resource_view_allocator.h"


class NENE_API gapi_online_resource_view_cache : noncopyable
{
public:
	//
	gapi_online_resource_view_cache(const std::shared_ptr<i::gapi_device>& device);
	
	// invalidate all cached resource views, called when the command list reset
	void reset();
	// initial the cache for a shader signature, called while binding pso
	void initialize(const gapi_shader_resource_tables& shader_resource_tables);
	// stage the cache of a resource view, called while binding resource 
	void stage_resource_view(gapi_shader_stage stage, uint32 reg, const std::shared_ptr<i::gapi_resource_view>& view);
	// commit all staged resource views in cache to GPU and make them online, called while draw | dispatch
	void commit_staged_resource_views(const std::shared_ptr<i::gapi_cmd_list>& cmd_list, const std::shared_ptr<i::gapi_device>& device);
	
protected:
	gapi_shader_resource_tables m_shader_resource_tables;
	std::vector<std::shared_ptr<i::gapi_resource_view>> m_staged_resource_views;
	std::shared_ptr<i::gapi_resource_view_allocator> m_online_resource_view_allocator;
};
