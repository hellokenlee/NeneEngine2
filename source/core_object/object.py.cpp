/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "object.h"
#include "pybind11/pybind11.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::class_<object>(m, "Object")
			.def(py::init<>())
			.def("func0", &object::func0)
			.def("func1", &object::func1)
			.def_static("test", &object::test)
			.def_readwrite("m_data0", &object::m_data0)
			.def_readonly("m_data1", &object::m_data1)
			.def_readwrite_static("s_data2", &object::s_data2)
			.def_readonly_static("s_data3", &object::s_data3)
		;
	}
}
