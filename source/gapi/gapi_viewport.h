/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


class gapi_viewport
{
public:
	virtual ~gapi_viewport() = default;

	virtual void set_rect(const rect& area) = 0;
};
