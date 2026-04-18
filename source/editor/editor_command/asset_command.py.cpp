/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::classh<asset_import_command, editor_command>(m, "AssetImportCommand")
			.def(py::init<const std::string&, const std::string&>())
			.def_static("supported_extensions", &asset_import_command::supported_extensions)
		;

		py::classh<asset_new_command, editor_command>(m, "AssetNewCommand")
			.def(py::init<const std::string&, const std::string&>())
		;
	}
}
