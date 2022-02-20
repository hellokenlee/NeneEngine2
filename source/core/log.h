/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyClangDiagnosticFormatNonliteral

#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iomanip>
#include <sstream>

#include <windows.h>

#include "platform.h"

#ifdef _UNICODE
	#define LOG(cat, level, fmt, ...) WLOG(cat, level, fmt, __VA_ARGS__)
#else
	#define LOG(cat, level, fmt, ...) SLOG(cat, level, fmt, __VA_ARGS__)
#endif

#define WLOG(cat, level, fmt, ...) wlog_impl(zznn_log_categort##cat::get_wide_name(), log_level::level, fmt, __VA_ARGS__)
#define SLOG(cat, level, fmt, ...) slog_impl(zznn_log_categort##cat::get_single_name(), log_level::level, fmt, __VA_ARGS__)


#define DECLARE_LOG_CATEGORY(cat_name) \
	class zznn_log_categort##cat_name : public log_category_base \
	{ \
	public: \
		inline static wstring get_wide_name() { return TEXT(#cat_name); } \
		inline static sstring get_single_name() { return (#cat_name); } \
	}; \


#define DEFINE_LOG_CATEGORY(cat_name) \
	zznn_log_categort##cat_name (cat_name);


enum class log_level
{
	info,
	warning,
	error,
	fatal,

	MAX_COUNT,
};

class log_category_base
{

};


void inline wlog_impl(const wstring& cat, const log_level& level, const wchar_t* const format, ...)
{
	static const wstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
		TEXT("info"),
		TEXT("warning"),
		TEXT("error"),
		TEXT("fatal"),
	};

	// The timestamp
	const time_type current = std::time(nullptr);
	time_struct current_time;
	platform::local_time(&current_time, &current);

	wstringstream text_stream;
	text_stream << std::put_time(&current_time, TEXT("[%y-%m-%d %H:%M:%S]"));
	const wstring time_string(text_stream.str());

	// The category and Level
	wprintf(TEXT("%s [%s] [%s] "), time_string.c_str(), cat.c_str(), log_levels[static_cast<int>(level)].c_str());

	// The actual log message
	va_list arg_list;

	va_start(arg_list, format);

	vwprintf(format, arg_list);

	va_end(arg_list);

	printf("\n");
}

void inline slog_impl(const sstring& cat, const log_level& level, const char* const format, ...)
{
	static const sstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
		"info",
		"warning",
		"error",
		"fatal",
	};

	// The timestamp
	const time_type current = std::time(nullptr);
	time_struct current_time;
	platform::local_time(&current_time, &current);

	sstringstream text_stream;
	text_stream << std::put_time(&current_time, "[%y-%m-%d %H:%M:%S]");
	const sstring time_string = text_stream.str();

	// The category and Level
	printf("%s [%s] [%s] ", time_string.c_str(), cat.c_str(), log_levels[static_cast<int>(level)].c_str());

	// The actual log message
	va_list arg_list;

	va_start(arg_list, format);

	vprintf(format, arg_list);

	va_end(arg_list);

	printf("\n");
}