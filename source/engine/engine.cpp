/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"

engine& engine::instance()
{
	static engine static_instance;
	return static_instance;
}