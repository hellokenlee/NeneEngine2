/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "log.h"
#include "interface/log_handler.h"

namespace nene
{
	/**
	 *  Log handler log to stdout
	 */
	class NENE_API stdout_log_handler : public log_handler
	{
	public:
		~stdout_log_handler() override = default;
		void emit(const std::string& log_message) override;
	};
}
