/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "log.h"

namespace i
{
	/**
	 *  Log handler
	 */
	class NENE_API log_handler
	{
	public:
		virtual ~log_handler() = default;
		virtual void emit(const std::string& log_message) {}
	};
}

/**
 *  Log handler log to stdout
 */
class NENE_API stdout_log_handler : public i::log_handler
{
public:
	~stdout_log_handler() override = default;
	void emit(const std::string& log_message) override;
};