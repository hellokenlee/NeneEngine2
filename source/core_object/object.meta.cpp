/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"
#include "object.h"


NMETA
{
	NCLASS("object", n::object)
		.constructor<int>()
		.method("func0", &n::object::func0)
		.method("func1", &n::object::func1)
	;
};