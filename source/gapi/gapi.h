/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

class gapi : public noncopyable
{
public:
	static void create(void* window);

	static void destroy();

public:
	static shared_ptr<gapi> m_instance;
};



void inline gapi_flush_resources() {};