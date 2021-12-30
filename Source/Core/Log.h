/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include <cstdarg>

#include "Platform.h"

#define LOG(cat, level, fmt, ...) log_impl(zznn_log_categort##cat::get_name(), log_level::level, fmt, __VA_ARGS__)


#define DEFINE_LOG_CATEGORY(cat) \
	class zznn_log_categort##cat : public log_category_base \
	{ \
	public: \
		static std::string get_name() { return #cat; } \
	}; \

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


void inline log_impl(const string& cat, const log_level& level, const char* format, ...)
{
	static const string LogLevelStrings[static_cast<int>(log_level::MAX_COUNT)] = {
		TEXT("info"),
		"warning",
		"error",
		"fatal",
	};

	// The timestamp
	static char time_stamp[256];
	time_type current = std::time(nullptr);
	time_struct current_time;
	platform::local_time(&current_time, &current);
	platform::strftime(time_stamp, sizeof(time_stamp), "[%y-%m-%d %H:%M:%S]", &current_time);

	// The category and Level
	printf("%s [%s] [%s] ", time_stamp, cat.c_str(), LogLevelStrings[static_cast<int>(level)].c_str());

	// The actual log message
	va_list arg_list;

	va_start(arg_list, format);

	vprintf(format, arg_list);

	va_end(arg_list);

	printf("\n");
}
