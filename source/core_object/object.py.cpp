/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "object.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::class_<object>(m, "Object")
			.def(py::init<>())
		;
	}
}
