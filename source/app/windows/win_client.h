/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <Windows.h>

#include "core/types.h"
#include "../client.h"


class win_client : public i::client
{
public:
	win_client() : win_client(TEXT("NeneEngine")) {}
	
	win_client(const string& name);
	
	virtual ~win_client() override;

public:
	virtual bool should_exit() override;

	virtual void poll_messages() override;

	virtual void* get_window() override { return static_cast<void*>(m_window); }

public:
	static bool m_client_should_exit;

protected:
	HWND m_window;
	HINSTANCE m_handle;
};
