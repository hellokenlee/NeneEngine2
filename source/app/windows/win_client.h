/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>

#include "core/types.h"
#include "../i_client.h"


class win_client : public i_client
{
public:
	win_client() : win_client(TEXT("NeneEngine")) {}
	
	win_client(const string& name);
	
	virtual ~win_client() override;

public:
	virtual bool should_exit() override;

	virtual void poll_messages() override;

	virtual void* get_window() override { return static_cast<void*>(m_window); }

protected:
	HWND m_window;
	HINSTANCE m_handle;
};
