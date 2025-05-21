/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <pybind11/embed.h>

#include "meta.h"
#include "object.h"
#include "pybind11/pybind11.h"


NMETA(object, m)
{
	t::n::class_<n::object>("object", m)
		.method("func0", &n::object::func0)
	;
}
