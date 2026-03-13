/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_import_command.h"
#include "core_object/py.h"

namespace nene
{
	PYBIND(m, 1)
	{
		py::class_<asset_import_command, editor_command>(m, "AssetImportCommand")
			.def(py::init<const std::string&, const std::string&>())
		;
	}
}
