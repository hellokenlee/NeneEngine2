/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_registry.h"

namespace nene::g
{
	PYBIND(m)
	{
		py::class_<asset_registry, std::unique_ptr<asset_registry, py::nodelete>>(m, "AssetRegistry")
			.def(py::init([]() { return &asset_registry::get(); }))
			.def("remove", static_cast<void(asset_registry::*)(const uuid&)>(&asset_registry::remove))
			.def("remove", [](asset_registry& self, const std::string& path) { self.remove(std::filesystem::path(path)); })
		;
	}
}
