/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "asset_abstract.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<asset_abstract>(m, "AssetAbstract")
			.def(py::init<>())
			.def_readonly("m_uuid", &asset_abstract::m_uuid)
			.def_readonly("m_type_name", &asset_abstract::m_type_name)
			.def_readonly("m_file_name", &asset_abstract::m_file_name)
		;
	}
}
