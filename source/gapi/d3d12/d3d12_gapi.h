/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "../gapi.h"

#include "d3d12_device.h"
#include "d3d12_viewport.h"

class d3d12_gapi : public gapi
{
public:
	d3d12_gapi(void* hwnd);
	~d3d12_gapi();

protected:
	shared_ptr<d3d12_device> m_device;
	shared_ptr<d3d12_viewport> m_viewport;
};