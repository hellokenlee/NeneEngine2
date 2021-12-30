/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "windows.h"

bool g_window_should_close = false;

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
		g_window_should_close = true;
		break;
	}
	default: 
	{
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
		0, 0, 0, 0, nullptr, nullptr,
		handle, nullptr
	);
}

client::~client()
{

}
