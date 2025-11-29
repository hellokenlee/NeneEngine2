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
	
	// 
	void reset();
	// initial the cache for a shader signature, called while binding pso
	void initialize(const gapi_shader_resource_table& shader_resource_table);
	// stage the cache of a resource view, called while binding resource 
	void stage_resource_view();
	// commit all resource views in cache to GPU and make them online, called while draw | dispatch
	void commit_staged_resource_views();
	
protected:
	std::shared_ptr<i::gapi_resource_view_allocator> m_online_resource_view_allocator;
};
