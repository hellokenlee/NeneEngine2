/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include "core/windll.h"

namespace nene
{
	class NENE_API serializable
	{
	public:
		//
		virtual ~serializable() = default;
		//
		virtual void serialize(archive& ar) = 0;
	};
}
