/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <pybind11/embed.h>

#include "meta.h"
#include "object.h"
#include "pybind11/pybind11.h"


NMETA(m)
{
	t::n::class_<n::object>("object", m)
		.constructor<int>()
		.method("func0", &n::object::func0)
		.method("func1", &n::object::func1)
		.static_method("test", &n::object::test)
		.property("m_data0", &n::object::m_data0)
		.property_readonly("m_data1", &n::object::m_data1)
		.static_property("s_data2", &n::object::s_data2)
		.static_property_readonly("s_data3", &n::object::s_data3)
	;
}
