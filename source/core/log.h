/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyClangDiagnosticFormatNonliteral

#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iomanip>
#include <sstream>

#include "platform.h"

#ifdef _UNICODE
	#define LOG(cat, level, fmt, ...) WLOG(cat, level, fmt, __VA_ARGS__)
	#define INTERCEPTE_LOG(log_func)  log_category_base::log_interceptions.push_back(log_func)
#else
	#define LOG(cat, level, fmt, ...) SLOG(cat, level, fmt, __VA_ARGS__)
#endif

#define WLOG(cat, level, fmt, ...) wlog_impl(zznn_log_category##cat::get_wide_name(), log_level::level, fmt, __VA_ARGS__)
#define SLOG(cat, level, fmt, ...) slog_impl(zznn_log_category##cat::get_single_name(), log_level::level, fmt, __VA_ARGS__)


#define DECLARE_LOG_CATEGORY(cat_name) \
	class zznn_log_category##cat_name : public log_category_base \
	{ \
	public: \
		inline static wstring get_wide_name() { return TEXT(#cat_name); } \
		inline static sstring get_single_name() { return (#cat_name); } \
	}; \


#define DEFINE_LOG_CATEGORY(cat_name) \
	zznn_log_category##cat_name (cat_name);


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
public:
	typedef decltype([](const string& timestamp, const string& cat, const string& lv, const string& message) -> void {}) log_function;
	static t::dynamic_array<log_function>  log_interceptions;
};

template<class tstring, class tstringstream, class tchar>
tstring log_impl_timestamp(const tchar* fmt)
{
	const time_type current = std::time(nullptr);
	time_struct current_time;
	platform::local_time(&current_time, &current);

	tstringstream text_stream;
	text_stream << std::put_time(&current_time, fmt);
	return {text_stream.str()};
}

template<class tstring>
const tstring& log_impl_loglevel(const log_level& level);

template<>
inline const sstring& log_impl_loglevel(const log_level& level)
{
	static const sstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
		"info",
		"warning",
		"error",
		"fatal",
	};
	return log_levels[static_cast<int>(level)];
}

template<>
inline const wstring& log_impl_loglevel(const log_level& level)
{
	static const wstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
		TEXT("info"),
		TEXT("warning"),
		TEXT("error"),
		TEXT("fatal"),
	};
	return log_levels[static_cast<int>(level)];
}

NENE_API void slog_impl(const sstring& cat, const log_level& level, const char* const format, ...);

NENE_API void wlog_impl(const wstring& cat, const log_level& level, const wchar_t* const format, ...);
