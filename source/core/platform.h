/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"


class NENE_API platform
{
public:

	static void local_time(std::tm* const tm, std::time_t const* const time);

	static std::vector<void*> get_windows(int32 pid=-1);
	
public:
	static constexpr uint32 max_path_length = 512;
};

