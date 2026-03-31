/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "static_mesh_asset.h"
#include "core_object/py.h"
#include "core_object/asset.h"


namespace nene::g
{
	PYBIND(m, 2)
	{
		py::class_<static_mesh_asset, asset, py::smart_holder>(m, "StaticMeshAsset")
			.def(py::init<>())
		;
	}
}
