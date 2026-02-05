/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"
#include "object.h"
#include "pybind11/pybind11.h"


namespace nene::g
{
	META(m)
	{
		class_<object>("Object", m)
			.constructor<>()
			.method("func0", &object::func0)
			.method("func1", &object::func1)
			.static_method("test", &object::test)
			.property("m_data0", &object::m_data0)
			.property_readonly("m_data1", &object::m_data1)
			.static_property("s_data2", &object::s_data2)
			.static_property_readonly("s_data3", &object::s_data3)
		;
	}
}
