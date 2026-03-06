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
		struct map 
		{};
		struct array
		{
			
		};
		//
		virtual ~archive() = default;
		//
		virtual map map() = 0;
		virtual array array() = 0;
		//
		virtual void serialize(const std::string& name, nullptr_t) = 0;
		virtual void serialize(const std::string& name, int32_t data) = 0;
		virtual void serialize(const std::string& name, float data) = 0;
		virtual void serialize(const std::string& name, const std::string& data) = 0;
		virtual void serialize(const std::string& name, const uuid& data) = 0;
	};
}
