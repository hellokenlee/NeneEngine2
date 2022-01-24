/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_context.h"

class gapi : public noncopyable
{
public:
	static void create(void* window);

	static void destroy();

	static shared_ptr<gapi> get() { CHECK(m_instance != nullptr); return m_instance; }

public:
	virtual void begin_drawing_viewport() = 0;

	virtual void end_drawing_viewport() = 0;


	virtual shared_ptr<gapi_cmd_context> create_cmd_context() = 0;

public:
	static shared_ptr<gapi> m_instance;
};


void inline clear_render_target() {};
