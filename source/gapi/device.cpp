/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "device.h"

#include "core/core.h"


device* device::m_device = nullptr;


device* device::get()
{
	CHECK(m_device != nullptr);
	return m_device;
}

void device::set(device* in_device)
{
	CHECK(m_device == nullptr);
	m_device = in_device;
}