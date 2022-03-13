/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"

#include "core/core.h"

#include "gapi/gapi.h"

#include "renderer/renderer_interface.h"
#include "renderer/simple_renderer.h"


DECLARE_LOG_CATEGORY(engine)


void engine_loop::init(void* window)
{
	//
	LOG(engine, info, TEXT("Engine Init!"));
	//
	gapi::create(window);
}

void engine_loop::update()
{
	gapi::get().start_frame();

	simple_renderer renderer;

	renderer.render_view_family();
	
	gapi::get().finish_frame();
}

void engine_loop::shutdown()
{
	//
	gapi::destroy();

	//
	LOG(engine, info, TEXT("Engine Shutdown!"));
}