/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi.h"
#include "core/core.h"
#include "d3d12/d3d12_device.h"

void create_gapi()
{
	bool use_d3d12 = true;

	if (use_d3d12)
	{
		device* instance = new d3d12_device();

		instance->init();

		device::set(instance);
	}
	else
	{
		CHECK(false);
	}
}

void destroy_gapi()
{
	device* instance = device::get();

	instance->shutdown();

	device::set(nullptr);
}

