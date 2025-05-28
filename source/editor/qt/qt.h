/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace qt
{
	class NENE_API binding
	{
	public:
		static binding& get();
		
		void initialize() const;
	};
}