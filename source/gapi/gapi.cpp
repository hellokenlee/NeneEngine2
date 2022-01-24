/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi.h"
#include "core/core.h"

#include "d3d12/gapi_d3d12.h"

shared_ptr<gapi> gapi::m_instance = nullptr;

void gapi::create(void* window)
{
	bool use_d3d12 = true;

	CHECK(m_instance == nullptr);

	if (use_d3d12)
	{
		m_instance = shared_ptr<d3d12_gapi>(new d3d12_gapi(window));
	}
	else
	{
		CHECK(false);
	}
}

void gapi::destroy()
{
	CHECK(m_instance != nullptr);

	m_instance.reset();

	m_instance = nullptr;
}

