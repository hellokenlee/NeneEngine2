/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"

#include "core/core.h"

#include "gapi/gapi.h"

#include "renderer/renderer_interface.h"
#include "renderer/simple_renderer.h"


DECLARE_LOG_CATEGORY(engine)

shared_ptr<renderer_interface> engine_loop::m_renderer = nullptr;

void engine_loop::init(void* window)
{
	//
	LOG(engine, info, TEXT("Engine Init!"));
	//
	gapi::create(window);
	//
	m_renderer = shared_ptr<simple_renderer>(new simple_renderer());
}

void engine_loop::update()
{
	gapi::get().start_frame();

	m_renderer->render_view_family();
	
	gapi::get().finish_frame();
}

void engine_loop::shutdown()
{
	//
	m_renderer.reset();

	gapi::destroy();

	//
	LOG(engine, info, TEXT("Engine Shutdown!"));
}