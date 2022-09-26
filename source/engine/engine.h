/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

class NENE_API engine
{
public:
	static engine& get();

private:
	engine() = default;
	~engine() = default;
};
