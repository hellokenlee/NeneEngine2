/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "string_utils.h"

#include <locale>
#include <codecvt>

namespace utils
{
	NENE_API std::wstring string_to_wstring(const std::string& in_string)
	{
#ifdef NENE_PLATFORM_WINDOWS
		int len = MultiByteToWideChar(CP_UTF8, 0, in_string.c_str(), -1, nullptr, 0);
		std::wstring result(len, 0);
		MultiByteToWideChar(CP_UTF8, 0, in_string.c_str(), -1, result.data(), len);
		return result;
#else  // NENE_PLATFORM_WINDOWS
		// ReSharper disable CppDeprecatedEntity
		std::wstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
		return result;
		// ReSharper restore CppDeprecatedEntity
#endif // NENE_PLATFORM_WINDOWS
	}

	NENE_API std::string wstring_to_string(const std::wstring& in_string)
	{
#ifdef NENE_PLATFORM_WINDOWS
		int len = WideCharToMultiByte(CP_UTF8, 0, in_string.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string result(len, 0);
		WideCharToMultiByte(CP_UTF8, 0, in_string.c_str(), -1, result.data(), len, nullptr, nullptr);
		return result;
#else  // NENE_PLATFORM_WINDOWS
		// ReSharper disable CppDeprecatedEntity
		std::string result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(in_string);
		return result;
		// ReSharper restore CppDeprecatedEntity
#endif // NENE_PLATFORM_WINDOWS
	}
}
