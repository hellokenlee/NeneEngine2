/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_asset.h"
#include "core_object/py.h"


namespace nene::g
{
	PYBIND(m, 3)
	{
		py::class_<material_asset, asset, py::smart_holder>(m, "MaterialAsset")
			.def(py::init<>())
			// smart_holder cannot `def_readwrite` export not smar_holder types
			.def_property("m_texture_bindings",
				[](const material_asset& self) { return self.m_texture_bindings; },
				[](material_asset& self, const texture_asset_handle& v) { self.m_texture_bindings = v; }
			)
		;
	}
}
