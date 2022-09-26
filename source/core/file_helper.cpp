/* Copyright reserved by KenLee@hellokenlee@163.com */

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "types.h"
#include "debug.h"
#include "file_helper.h"

#include <locale>
#include <codecvt>


namespace file_helper
{
	wstring sstring_to_wstring(const sstring& in_string)
	{
		wstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
		return result;
	}

	sstring wstring_to_sstring(const wstring& in_string)
	{
		sstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(in_string);
		return result;
	}
	
}
