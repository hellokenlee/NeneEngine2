/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "serializable.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::class_<serializable>(m, "Serializable")
			.def(py::init<>())
		;
	}
}
