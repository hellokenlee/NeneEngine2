/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyPerformanceNoIntToPtr

#include "win_client.h"
#include "core/core.h"
#include "engine/engine_loop.h"


static LRESULT CALLBACK WindowProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	win_client* p_client = reinterpret_cast<win_client*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	switch (msg) 
	{
		case WM_CREATE:
		{
			const LPCREATESTRUCT p_create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_create_struct->lpCreateParams));
			break;
		}
		case WM_DESTROY: 
		{
			PostQuitMessage(0);
			break;
		}
		case WM_PAINT:
		{
			if (engine_loop::is_initialized())
			{
				engine_loop::tick();
			}
			break;
		}
		case WM_SIZE:
		{
			if (engine_loop::is_initialized())
			{
				RECT window_rect = {};
				::GetWindowRect(hWnd, &window_rect);
				uint2 window_size(static_cast<uint32_t>(window_rect.right - window_rect.left), static_cast<uint32_t>(window_rect.bottom - window_rect.top));
				engine_loop::resize(window_size);
			}
			
			break;
		}
		case WM_CLOSE: 
		{
			p_client->m_client_should_exit.store(true);
			break;
		}
		default: 
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}


win_client::win_client(const std::wstring& name)
	: m_window(nullptr)
	, m_handle(nullptr)
{
	constexpr auto DEFAULT_WINDOW_WIDTH = 800u;
	constexpr auto DEFAULT_WINDOW_HEIGHT = 600u;
	//
	m_handle = GetModuleHandle(nullptr);

	static LPCWSTR window_class_name = TXT("NeneEngineWindowClass");

	//
	HICON icon = static_cast<HICON>(LoadImage(nullptr, L"content/engine/sakura.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	CHECK(icon != nullptr);

	WNDCLASSEX window_class = {};
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = &WindowProcessFunction;
	window_class.hInstance = m_handle;
	window_class.hIcon = icon;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	window_class.lpszClassName = window_class_name;
	window_class.hIconSm = icon;
	//
	::RegisterClassEx(&window_class);
	//
	m_window = CreateWindow(
		window_class.lpszClassName, 
		name.c_str(), 
		WS_OVERLAPPEDWINDOW,
		0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 
		nullptr, nullptr,
		m_handle, this
	);
	//
	::ShowWindow(m_window, SW_SHOW);

	//
	engine_loop::initialize(m_window, uint2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
}

win_client::~win_client()
{
	engine_loop::shutdown();
}

bool win_client::should_exit()
{
	return m_client_should_exit.load();
}

void win_client::poll_message()
{
	MSG msg;
	PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
	if (msg.message == WM_QUIT)
	{
		m_client_should_exit.store(true);
	}
	else
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);			
	}
}