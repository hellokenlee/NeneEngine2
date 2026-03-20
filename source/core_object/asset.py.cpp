/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "asset.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<asset>(m, "Asset")
			.def(py::init<>())
			.def_readwrite("m_uuid", &asset::m_uuid)
		;
	}
}
