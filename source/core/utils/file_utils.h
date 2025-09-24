/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/types.h"

namespace utils
{
	NENE_API std::string load_file_to_string(const std::string& filepath);
	
	NENE_API std::wstring load_file_to_wstring(const std::string& filepath);
}
