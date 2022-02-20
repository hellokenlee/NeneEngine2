/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"

#include "core/core.h"

#include "gapi/gapi.h"

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
	gapi::get()->begin_drawing_viewport();
	gapi::get()->end_drawing_viewport();
}

void engine_loop::shutdown()
{
	//
	gapi::destroy();

	//
	LOG(engine, info, TEXT("Engine Shutdown!"));
}