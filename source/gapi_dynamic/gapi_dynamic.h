/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "gapi/gapi_resource.h"
#include "gapi_cmd_context.h"


enum class gapi_platform
{
	direct3d12,
	vulkan,
	metal,
};

/**
 *	A singleton instance that route call graphics api.
 */
class NENE_API gapi_dynamic : noncopyable
{
public:
	// Router functions
	
	t::shared_ptr<i::gapi_vertex_shader> create_vertex_shader(const sstring& filepath, const sstring& entry);

	t::shared_ptr<i::gapi_pixel_shader> create_pixel_shader(const sstring& filepath, const sstring& entry);

	t::shared_ptr<i::gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc);

	t::shared_ptr<i::gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc);

	t::shared_ptr<gapi_cmd_context> get_cmd_context(const uint32& thread_id = 0);
	
public:
	// Singleton methods
	
	/** Fetch the current gapi that is using. */
	static t::shared_ptr<gapi_dynamic> get();
	
	/** Create adapter and device from a window handler. */
	static void create(void* window);

	/** Clean up the device and adapter. */
	static void destroy();


protected:
	gapi_dynamic(const gapi_platform& platform, void* window);

	~gapi_dynamic() override;

	t::shared_ptr<i::gapi_factory> m_factory;
	t::shared_ptr<i::gapi_adapter> m_adapter;
	t::shared_ptr<i::gapi_device> m_device;
	
	t::shared_ptr<i::gapi_swap_chain> m_swap_chain;
};
