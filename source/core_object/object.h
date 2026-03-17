/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/windll.h"
#include "archive/archive.h"


namespace nene::g
{
	class NENE_API object
	{
	public:
		//
		virtual ~object() = default;
		//
		virtual void serialize(archive& ar);
	};
}
