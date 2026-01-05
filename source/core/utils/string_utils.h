/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include "core/windll.h"

namespace utils
{
	NENE_API std::wstring string_to_wstring(const std::string& in_string);

	NENE_API std::string wstring_to_string(const std::wstring& in_string);

	NENE_API void string_replace(std::string& s, std::string_view from, std::string_view to);
}