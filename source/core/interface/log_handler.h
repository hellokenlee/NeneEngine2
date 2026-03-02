/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include "windll.h"

namespace nene
{
	/**
	 *  Log handler abstract base class
	 */
	class NENE_API log_handler
	{
	public:
		virtual ~log_handler() = default;
		virtual void emit(const std::string& log_message) = 0;
	};
}