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
		virtual std::shared_ptr<archive> emplace_map(const std::string& name) = 0;
		virtual std::shared_ptr<archive> emplace_array(const std::string& name) = 0;
		virtual void emplace(const std::string& name, nullptr_t) = 0;
		virtual void emplace(const std::string& name, int32_t data) = 0;
		virtual void emplace(const std::string& name, float data) = 0;
		virtual void emplace(const std::string& name, const std::string_view& data) = 0;
		virtual void emplace(const std::string& name, const uuid& data) = 0;
	};
}
