/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"


namespace i
{
	class serializable
	{
	public:
		virtual ~serializable() = default;
		virtual void serialize(archive& ar) = 0;
	};
}
