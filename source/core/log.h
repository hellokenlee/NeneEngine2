/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyClangDiagnosticFormatNonliteral

#pragma once

#include <set>
#include <queue>
#include <string>
#include <chrono>
#include <format>
#include <source_location>

#include "platform.h"

namespace i
{
	class log_handler;	
}

/**
 * Level of each log
 */
enum log_level : uint8_t
{
	fatal = 0, 
	warn, 
	error,
	info,
};

/**
 *	Macro free logger
 */
class NENE_API logger final 
{
public:
	logger(const std::string_view& name);
	logger(const logger&) = delete;
	logger(logger&&) = delete;
	~logger() = default;

	/** Main log method */
	void log(const log_level& level, const std::string_view& message) const;

	/** Configurate log level */	
	void set_level(const log_level& level) { m_enabled_level = level; }
	bool should_log(const log_level& level) const { return m_enabled_level >= level; }

	/** Handler operations */
	static void add_handler(const std::shared_ptr<i::log_handler>& handler);
	static void remove_handler(const std::shared_ptr<i::log_handler>& handler);
	
protected:
	//
	std::string m_name;
	log_level m_enabled_level = info;
	//
	static std::set<std::shared_ptr<i::log_handler>> s_handlers;
};

/**
 * Usage:
 *		log(engine, info, "something wrong: %s", str);
 *		
 * Outputs:
 *		[yyyy-mm-dd hh:mm:ss] [engine] [info] something wrong: xxxx
 */
template <typename ...format_arg_ts>
void log(const logger& logger_instance, const log_level& level, std::format_string<format_arg_ts...> fmt, format_arg_ts&&... format_args)
{
	if (logger_instance.should_log(level))
	{
		logger_instance.log(level, std::format(fmt, std::forward<format_arg_ts>(format_args)...));		
	}
}

/**
 * Usage:
 *		log(logger, info, "we have a problem here");
 *
 * Outputs:
 *		[yyyy-mm-dd hh:mm:ss] xxx.cpp(12): `some_func(...)`: we have a problem here
 */
inline void trace(const logger& logger_instance, const log_level& level, const std::string_view& message, const std::source_location& location = std::source_location::current())
{
	if (logger_instance.should_log(level))
	{
		logger_instance.log(level, std::format("{}({}): `{}(...)`: {}", location.file_name(), location.line(), location.function_name(), message));
	}
}
