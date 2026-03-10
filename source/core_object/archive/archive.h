/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace nene
{
	/***
	 *	The writer/reader abstract base class
	 */
	class NENE_API archive
	{
	public:
		//
		virtual ~archive() = default;
		
		//
		virtual void process(const std::string& name, nullptr_t) = 0;
		virtual void process(const std::string& name, int32_t& data) = 0;
		virtual void process(const std::string& name, float& data) = 0;
		virtual void process(const std::string& name, std::string& data) = 0;
		virtual void process(const std::string& name, uuid& data) = 0;
		//
		virtual bool is_loading() const = 0;
		
		//
		virtual void enter_section(const std::string& name) = 0;
		virtual void leave_section() = 0;
	};
}
