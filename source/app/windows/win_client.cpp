/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "win_client.h"

#include "windows.h"

#include "core/core.h"

bool win_client::m_client_should_exit = false;


static LRESULT CALLBACK WindowProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	win_client* p_client = reinterpret_cast<win_client*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (msg) 
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT p_create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_create_struct->lpCreateParams));
		break;
	}
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		break;
	}
	case WM_CLOSE: 
	{
		win_client::m_client_should_exit = true;
		break;
	}
	default: 
	{
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


win_client::win_client(const string& name)
	: m_handle(nullptr)
	, m_window(nullptr)
{
	//
	HINSTANCE m_handle = GetModuleHandle(0);

	static LPCWSTR window_class_name = TEXT("NeneEngineClass");

	WNDCLASSEX window_class = {};
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProcessFunction;
	window_class.hInstance = m_handle;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.lpszClassName = window_class_name;
	//
	RegisterClassEx(&window_class);
	//
	m_window = CreateWindow(
		window_class.lpszClassName, 
		name.c_str(), 
		WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, 
		nullptr, nullptr,
		m_handle, this
	);
	//
	ShowWindow(m_window, SW_SHOW);
}

win_client::~win_client()
{
	
}

bool win_client::should_exit()
{
	return m_client_should_exit;
}

void win_client::poll_messages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}