/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>
#include <vector>
#include <ctime>
#include "core/windll.h"


class NENE_API platform
{
public:
	static void local_time(std::tm* tm, std::time_t const* time);

	static std::vector<void*> get_windows(int32_t pid=-1);
	
public:
	static constexpr uint32_t max_path_length = 512;
};
