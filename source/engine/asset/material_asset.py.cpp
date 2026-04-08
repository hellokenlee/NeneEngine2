/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_asset.h"
#include "core_object/py.h"


namespace nene::g
{
	PYBIND(m, 2)
	{
		py::class_<material_asset, asset, py::smart_holder>(m, "MaterialAsset")
			.def(py::init<>())
		;
	}
}
