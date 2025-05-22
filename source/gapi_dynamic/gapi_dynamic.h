/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_factory.h"
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
	/** Get the context for a thread. */
	std::shared_ptr<gapi_cmd_context> get_cmd_context(const uint32& thread_id = 0);

	/** Shader creations. */
	std::shared_ptr<i::gapi_vertex_shader> create_vertex_shader(const std::string& filepath, const std::string& entry) const;

	std::shared_ptr<i::gapi_pixel_shader> create_pixel_shader(const std::string& filepath, const std::string& entry) const;

	/** Pipeline state creations. */
	std::shared_ptr<i::gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) const;

	std::shared_ptr<i::gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) const;

	/** Resource creations. */
	std::shared_ptr<i::gapi_buffer> create_buffer(const gapi_resource_desc& desc) const;
	
	std::shared_ptr<i::gapi_texture> create_texture(const gapi_resource_desc& desc) const;

	/** Engine helpers. */
	void start_frame();

	void finish_frame();

	std::shared_ptr<i::gapi_texture> get_back_buffer_texture() const;

public:
	/** Clean up the gapi. */
	~gapi_dynamic() override;

	/** Create adapter and device from a window handler. */
	static void create(void* window);
	
	/** Fetch the current gapi that is using. */
	static const std::shared_ptr<gapi_dynamic>& get();

protected:
	/** Internal constructor. */
	gapi_dynamic(const gapi_platform& platform, void* window);
	
	std::shared_ptr<i::gapi_factory> m_factory;
	std::shared_ptr<i::gapi_adapter> m_adapter;
	std::shared_ptr<i::gapi_device> m_device;
	std::shared_ptr<i::gapi_swap_chain> m_swap_chain;

	std::vector<std::shared_ptr<gapi_cmd_context>> m_cmd_contexts;
};
