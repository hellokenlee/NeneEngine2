/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>

#include "core/types.h"


class client
{
public:
	client() : client(TEXT("NeneEngine")) {}
	
	client(const string& name);
	
	~client();

	void poll_events();

public:
	static bool s_should_exit;

protected:
	HWND window;
	HINSTANCE handle;
};
