/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_manager.h"
#include "gapi_d3d12/gapi_d3d12.h"

t::shared_ptr<gapi> gapi_manager::m_instance = nullptr;

void gapi_manager::create(void* window)
{
	bool use_d3d12 = true;

	CHECK(m_instance == nullptr);

	if (use_d3d12)
	{
		m_instance = t::make_shared<gapi_d3d12>(window);
	}
	else
	{
		CHECK(false);
	}
}

void gapi_manager::initialize(const gapi_platform& platform, void* device)
{
	CHECK(device != nullptr);
	CHECK(m_instance == nullptr);

	switch (platform)
	{
	case gapi_platform::direct3d12:
	{
		m_instance = t::make_shared<gapi_d3d12>(static_cast<ID3D12Device*>(device));
		break;
	}
	case gapi_platform::vulkan:
	case gapi_platform::metal:
	{
		CHECK(false);
		break;
	}
	default:
	{
		CHECK(false);
	}
	}
}

void gapi_manager::destroy()
{
	CHECK(m_instance != nullptr);

	m_instance.reset();

	m_instance = nullptr;
}

t::shared_ptr<gapi> gapi_manager::get()
{
	return m_instance;
}
