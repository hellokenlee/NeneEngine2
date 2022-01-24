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

public:
	virtual void begin_drawing_viewport() override;

	virtual void end_drawing_viewport() override;

	virtual shared_ptr<gapi_cmd_context> create_cmd_context() override;

protected:
	shared_ptr<d3d12_device> m_device;
	shared_ptr<d3d12_viewport> m_viewport;
};