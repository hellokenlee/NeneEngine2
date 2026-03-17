/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "archive.h"
#include "core_object/object.h"


namespace nene
{
	archive& archive::operator<<(g::object& obj)
	{
		obj.serialize(*this);
		return *this;
	}
}
