/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"

engine& engine::get()
{
	static engine instance;
	return instance;
}