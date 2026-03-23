/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyClangDiagnosticFormatNonliteral

#pragma once

#include <string>
#include <chrono>
#include <format>
#include <source_location>

#include "platform.h"
#include "event_id.h"
#include "event_publish_subscribe.h"

namespace nene
{
	struct log_message_event : typed_event<event_id::log_message_event>
	{
		explicit log_message_event(std::string message)
			: m_message(std::move(message))
		{}

		std::string m_message;
	};

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

		/** Handlers are event_listener; on_notify receives nene::log_message_event */
		static event_publisher& publisher();

	protected:
		//
		std::string m_name;
		log_level m_enabled_level = info;
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
}