/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace nene
{
	/***
	 *	The writer/reader abstract base class
	 */
	class archive
	{
	public:
		//
		virtual ~archive() = default;
		//
    	virtual void serialize(void *data, const size_t& length) = 0;
    	virtual void serialize(const int& data) = 0;
    	virtual void serialize(const bool& data) = 0;
		virtual void serialize(const std::string& data) = 0;
	};
}
