/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_shader.h"
#include "gapi_resource.h"
#include "gapi_pipeline_state.h"
#include "gapi_cmd_context.h"
#include "gapi_viewport.h"


class gapi
{
public:
	// >>> Static Methods >>>
	static void create(void* window);

	static void destroy();

	static shared_ptr<gapi> get() { CHECK(m_instance != nullptr); return m_instance; }

	// <<< Static Methods <<<

public:
	//
	virtual shared_ptr<gapi_cmd_context> get_context(const int32 id=-1) = 0;

	// >>> View Port Related >>>

	virtual void start_frame() = 0;

	virtual void finish_frame() = 0;

	virtual shared_ptr<gapi_viewport> get_viewport() = 0;

	// <<< View Port Related <<<


	// >>> Shader Related >>>

	virtual shared_ptr<gapi_vertex_shader> create_vertex_shader(const gapi_shader_initializer& initializer) = 0;

	virtual shared_ptr<gapi_pixel_shader> create_pixel_shader(const gapi_shader_initializer& initializer) = 0;

	// <<< Shader Related <<<


	// >>> Pipeline State Related >>>

	virtual shared_ptr<gapi_compute_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_initializer&) = 0;

	virtual shared_ptr<gapi_graphics_pipeline_state> create_graphic_pipeline_state(const gapi_graphics_pipeline_state_initializer&) = 0;

	
	// <<< Pipeline State Related <<<


	// >>> Resource Related >>>

	virtual shared_ptr<gapi_vertex_buffer> create_vertex_buffer(const size_t& buffer_stride, const size_t& buffer_size, const gapi_resource_usage& buffer_usage) = 0;

	virtual void* lock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer) = 0;

	virtual void unlock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer) = 0;

	// <<< Resource Related <<<

public:
	gapi() = default;

	virtual ~gapi() = default;

	gapi(gapi& other) = delete;

	gapi& operator=(const gapi& other) = delete;

protected:
	static shared_ptr<gapi> m_instance;
};
