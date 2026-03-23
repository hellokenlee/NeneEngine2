/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <chrono>


namespace nene::g
{
	/** basic unit for serialization */
	class NENE_API level
	{
	public:
		level();
		void update(std::chrono::milliseconds delta);
		
	protected:
	};
}