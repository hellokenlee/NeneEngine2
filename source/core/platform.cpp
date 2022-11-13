/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "platform.h"
#include "core.h"
#include <ctime>

DECLARE_LOG_CATEGORY(platform)

void platform::local_time(time_struct* const tm, time_type const* const time)
{
#ifdef _MSVC_LANG
	localtime_s(tm, time);
#else

#endif
}

void platform::strftime(char* buffer, size_t buffer_size, const char* fmt, const time_struct* tm)
{
	std::strftime(buffer, buffer_size, "[%y-%m-%d %H:%M:%S]", tm);
}


t::dynamic_array<void*> platform::get_windows(int32 pid)
{
	t::dynamic_array<void*> results;
#if defined(_WIN32) || defined(_WIN64)
	if (pid < 0)
	{
		pid = static_cast<int32>(GetCurrentProcessId());
	}
	HWND curr_window = nullptr;
	
	do
	{
		curr_window = FindWindowEx(nullptr, curr_window, nullptr, nullptr);
		DWORD curr_pid = 0;
		GetWindowThreadProcessId(curr_window, &curr_pid);
		if (curr_pid == pid)
		{
			results.push_back(static_cast<void*>(curr_window));
		}
	}
	while (curr_window != nullptr);
#endif
	return results;
}