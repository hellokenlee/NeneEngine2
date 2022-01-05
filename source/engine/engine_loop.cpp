/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"

DEFINE_LOG_CATEGORY(engine)


void engine_loop::init()
{
	LOG(engine, info, "Engine Init!");
}

void engine_loop::update()
{
}

void engine_loop::shutdown()
{
	LOG(engine, info, "Engine Shutdown!");
}