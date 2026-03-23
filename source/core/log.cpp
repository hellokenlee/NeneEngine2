/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "log.h"
#include "debug.h"
#include <chrono>
#include <magic_enum/magic_enum.hpp>

namespace nene
{
	event_publisher& logger::publisher()
	{
		static event_publisher s_instance;
		return s_instance;
	}

	logger::logger(const std::string_view& name)
		: m_name(name)
	{
	}

	void logger::log(const log_level& level, const std::string_view& message) const
	{
		// FORMAT: 2025-09-18 04:58:10 info [object] message,message,message
		std::chrono::zoned_time local_now(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
		auto now = std::format("{:%F %T}", local_now);
		std::string log_message = std::format("{} {} [{}] {}", now,  magic_enum::enum_name(level), m_name, message);

		log_message_event log_event(std::move(log_message));
		publisher().notify(log_event);
	}
}