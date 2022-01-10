/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>

#include "core/types.h"
#include "../iclient.h"


class win_client : public iclient
{
public:
	win_client() : win_client(TEXT("NeneEngine")) {}
	
	win_client(const string& name);
	
	virtual ~win_client() override;

public:
	virtual bool should_exit() override;

	virtual void poll_messages() override;

protected:
	HWND m_window;
	HINSTANCE m_handle;
};
