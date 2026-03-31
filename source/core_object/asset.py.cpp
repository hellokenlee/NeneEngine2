/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "asset.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<asset, py::smart_holder>(m, "Asset")
			.def_readwrite("m_uuid", &asset::m_uuid)
		;
	}
}
