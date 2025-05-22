/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"
#include "log.h"
#include "debug.h"


DECLARE_LOG_CATEGORY(filehelper)


namespace file_helper
{
	NENE_API std::string load_file_to_string(const std::string& filepath);
	
	NENE_API std::wstring load_file_to_wstring(const std::string& filepath);
	
	NENE_API std::wstring string_to_wstring(const std::string& in_string);

	NENE_API std::string wstring_to_string(const std::wstring& in_string);
}
