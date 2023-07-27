/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_dynamic.h"
#include "gapi_d3d12/gapi_d3d12_factory.h"

t::console_var cvar_gapi_platform(
	"gapi.platform",
	0,
	"Select the platform that engine use:\n"
	"	0: d3d12;\n"
	"	1: vulkan;\n"
	"	2: metal;\n"
);

t::shared_ptr<gapi_dynamic> g_gapi_instance = nullptr;

gapi_dynamic::gapi_dynamic(const gapi_platform& platform, void* window)
	: m_factory(nullptr)
	, m_adapter(nullptr)
	, m_device(nullptr)
	, m_swap_chain(nullptr)
{
	switch (platform)
	{
	case gapi_platform::direct3d12:
		{
			m_factory = t::make_shared<gapi_d3d12_factory>(static_cast<HWND>(window));
			m_adapter = m_factory->create_adapter();
			m_device = m_adapter->create_device();
			m_swap_chain = m_factory->create_swap_chain(window, point32{800, 600});
			break;
		}
	case gapi_platform::vulkan:
	case gapi_platform::metal:
	default:
		{
			CHECK(false);
		}
	}
}

gapi_dynamic::~gapi_dynamic()
{
	m_swap_chain.reset();
	m_device.reset();
	m_adapter.reset();
	m_factory.reset();
}

void gapi_dynamic::create(void* window)
{
	CHECK(window != nullptr);
	CHECK(g_gapi_instance == nullptr);

	const auto platform = static_cast<gapi_platform>(cvar_gapi_platform.get_value_thread_unsafe());
	g_gapi_instance = t::make_shared<gapi_dynamic>(platform, static_cast<HWND>(window));

}

void gapi_dynamic::destroy()
{
	CHECK(g_gapi_instance != nullptr);

	g_gapi_instance.reset();
}

t::shared_ptr<gapi_dynamic> gapi_dynamic::get()
{
	CHECK(g_gapi_instance != nullptr);
	
	return g_gapi_instance;
}
