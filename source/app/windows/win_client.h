/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <Windows.h>

#include "core/types.h"
#include "../client.h"


class win_client : public i::client
{
public:
	win_client() : win_client(TXT("NeneEngine")) {}
	win_client(const std::wstring& name);
	~win_client() override;
	
	bool should_exit() override;

	void poll_message() override;

	void* get_window() override { return static_cast<void*>(m_window); }

public:
	std::atomic_bool m_client_should_exit = false;

protected:
	HWND m_window;
	HINSTANCE m_handle;
};
