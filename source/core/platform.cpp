/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "platform.h"
#include "core.h"
#include <ctime>


void platform::local_time(std::tm* const tm, std::time_t const* const time)
{
#ifdef _MSVC_LANG
	localtime_s(tm, time);
#else
	NOT_IMPLEMENTED();
#endif
}

std::vector<void*> platform::get_windows(int32 pid)
{
	std::vector<void*> results;
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