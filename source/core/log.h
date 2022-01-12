/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iomanip>
#include <sstream>

#include <windows.h>

#include "platform.h"

#define LOG(cat, level, fmt, ...) log_impl(zznn_log_categort##cat::get_name(), log_level::level, TEXT(fmt), __VA_ARGS__)

#define DEFINE_LOG_CATEGORY(cat) \
	class zznn_log_categort##cat : public log_category_base \
	{ \
	public: \
		static string get_name() { return TEXT(#cat); } \
	}; \

#define EXTERN_LOG_CATEGORY(cat)

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


void inline log_impl(const string& cat, const log_level& level, const wchar_t* format, ...)
{
	static const string LogLevelStrings[static_cast<int>(log_level::MAX_COUNT)] = {
		TEXT("info"),
		TEXT("warning"),
		TEXT("error"),
		TEXT("fatal"),
	};

	// The timestamp
	time_type current = std::time(nullptr);
	time_struct current_time;
	platform::local_time(&current_time, &current);

	stringstream text_stream;
	text_stream << std::put_time(&current_time, TEXT("[%y-%m-%d %H:%M:%S]"));
	string time_string = text_stream.str();

	// The category and Level
	wprintf(TEXT("%s [%s] [%s] "), time_string.c_str(), cat.c_str(), LogLevelStrings[static_cast<int>(level)].c_str());

	// The actual log message
	va_list arg_list;

	va_start(arg_list, format);

	vwprintf(format, arg_list);

	va_end(arg_list);

	printf("\n");
}
