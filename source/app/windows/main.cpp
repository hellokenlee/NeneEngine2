/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "Windows.h"
#include "core/log.h"
#include "engine/engine_loop.h"

int main()
{
	engine_loop::initialize();

	engine_loop::update();

	engine_loop::shutdown();

	return 0;
	
}
