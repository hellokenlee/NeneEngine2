/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"

#include "core/core.h"

#include "gapi/gapi.h"

DEFINE_LOG_CATEGORY(engine)


void engine_loop::init(void* window)
{
	//
	gapi::create(window);

	//
	LOG(engine, info, "Engine Init!");
}

void engine_loop::update()
{
	int a = 1;
}

void engine_loop::shutdown()
{
	//
	gapi::destroy();

	//
	LOG(engine, info, "Engine Shutdown!");
}