/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>

#include "core/types.h"


class client
{
public:
	client() : client("NeneEngine") {}
	
	client(const string& name);
	
	~client();

protected:
	HWND window;
	HINSTANCE handle;
};
