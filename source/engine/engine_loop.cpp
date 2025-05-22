/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "core/core.h"
#include "gapi_dynamic/gapi_dynamic.h"
#include "renderer/renderer.h"
#include "renderer/simple_renderer.h"


DECLARE_LOG_CATEGORY(engine)
DEFINE_LOG_CATEGORY(engine)

std::shared_ptr<i::renderer> engine_loop::m_renderer = nullptr;

void engine_loop::init(void* window)
{
	//
	LOG(engine, info, TXT("Engine Init!"));
	//
	gapi_dynamic::create(window);
	//
	m_renderer = std::make_shared<simple_renderer>();
}

void engine_loop::update()
{
	gapi_dynamic::get()->start_frame();

	m_renderer->render_view_family(gapi_dynamic::get()->get_back_buffer_texture());
	
	gapi_dynamic::get()->finish_frame();
}

void engine_loop::shutdown()
{
	// Waiting for executing all commands
	gapi_dynamic::get()->start_frame();
	gapi_dynamic::get()->finish_frame();

	//
	m_renderer.reset();
	
	//
	LOG(engine, info, TXT("Engine Shutdown!"));
}
