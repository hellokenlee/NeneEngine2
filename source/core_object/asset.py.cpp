/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "asset.h"


namespace nene::g
{
	PYBIND(m)
	{
		asset a;
		
		py::class_<asset>(m, "Asset")
			.def(py::init<>())
			.def_readwrite("m_uuid", &asset::m_uuid)
		;
	}
}
