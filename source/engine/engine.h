/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/types.h"

class engine
{
public:
	static engine& instance();

private:
	engine() = default;
	~engine() = default;
};
