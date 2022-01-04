/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "windows.h"

static LRESULT CALLBACK WindowProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE: 
	{
		client::s_should_exit = true;
		break;
	}
	default: 
	{
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool client::s_should_exit = false;

client::client(const string& name)
	: handle(nullptr)
	, window(nullptr)
{
	//
	HINSTANCE handle = GetModuleHandle(0);

	static LPCWSTR window_class_name = TEXT("NeneEngineClass");

	WNDCLASSEX window_class = {};
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProcessFunction;
	window_class.hInstance = handle;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.lpszClassName = window_class_name;
	//
	RegisterClassEx(&window_class);
	//
	window = CreateWindowEx(
		0, window_class_name, name.c_str(), WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, nullptr, nullptr,
		handle, nullptr
	);
	//
	ShowWindow(window, SW_SHOW);
}

client::~client()
{
	
}


void client::poll_events()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}