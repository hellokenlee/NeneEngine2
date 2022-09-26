/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "core/core.h"
#include "gapi_dynamic/gapi_manager.h"
#include "renderer/i_renderer.h"
#include "renderer/simple_renderer.h"


DECLARE_LOG_CATEGORY(engine)

shared_ptr<i_renderer> engine_loop::m_renderer = nullptr;

void engine_loop::init(void* window)
{
	//
	LOG(engine, info, TEXT("Engine Init!"));
	//
	gapi_manager::create(window);
	//
	m_renderer = shared_ptr<simple_renderer>(new simple_renderer());
}

void engine_loop::update()
{
	gapi_manager::get()->start_frame();

	m_renderer->render_view_family();
	
	gapi_manager::get()->finish_frame();
}

void engine_loop::shutdown()
{
	// Waiting for executing all commands
	gapi_manager::get()->start_frame();
	gapi_manager::get()->finish_frame();

	//
	m_renderer.reset();

	gapi_manager::destroy();

	//
	LOG(engine, info, TEXT("Engine Shutdown!"));
}