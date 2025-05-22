/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"


namespace n
{
	class serializable
	{
	public:
		void serialize(i::archive& ar) const;
	};
}
