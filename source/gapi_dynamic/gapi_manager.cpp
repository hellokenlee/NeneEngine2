/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_manager.h"
#include "gapi_d3d12/gapi_d3d12.h"

shared_ptr<gapi> gapi_manager::m_instance = nullptr;

void gapi_manager::create(void* window)
{
	bool use_d3d12 = true;

	CHECK(m_instance == nullptr);

	if (use_d3d12)
	{
		m_instance = shared_ptr<gapi_d3d12>(new gapi_d3d12(window));
	}
	else
	{
		CHECK(false);
	}
}

void gapi_manager::destroy()
{
	CHECK(m_instance != nullptr);

	m_instance.reset();

	m_instance = nullptr;
}

shared_ptr<gapi> gapi_manager::get()
{
	CHECK(m_instance != nullptr);
	return m_instance;
}
