/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

class engine
{
public:
	static engine& get();

private:
	engine() = default;
	~engine() = default;
};
