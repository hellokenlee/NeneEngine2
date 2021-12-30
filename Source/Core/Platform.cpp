/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "platform.h"

#include <ctime>

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
