/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_asset.h"
#include "core_object/py.h"


namespace nene::g
{
	PYBIND(m, 3)
	{
		py::class_<material_asset, asset, py::smart_holder>(m, "MaterialAsset")
			.def(py::init<>())
			.def_readwrite("m_texture_bindings", &material_asset::m_texture_bindings)
			.def_readwrite("m_scalar_parameters", &material_asset::m_scalar_parameters)
		;
	}
}
