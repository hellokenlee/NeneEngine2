/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"


class NENE_API platform
{
public:

	static void local_time(time_struct* const tm, time_type const* const time);
	
	static void strftime(char* buffer, size_t buffer_size, const char* fmt, const time_struct* tm);

	static t::dynamic_array<void*> get_windows(int32 pid=-1);
	
public:
	static constexpr uint32 max_path_length = 512;
};

