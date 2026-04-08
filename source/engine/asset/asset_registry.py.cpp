/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_registry.h"


namespace nene::g
{
	PYBIND(m, 1)
	{
		py::class_<asset_registry, std::unique_ptr<asset_registry, py::nodelete>>(m, "AssetRegistry")
			.def(py::init([]() { return &asset_registry::get(); }))
			.def("remove", static_cast<void(asset_registry::*)(const uuid&)>(&asset_registry::remove))
			.def("remove", [](asset_registry& self, const std::string& path) { self.remove(std::filesystem::path(path)); })
 			.def("find_abstract", [](asset_registry& self, const std::string& path) { return self.find_abstract(std::filesystem::path(path)); })
			.def("find_abstract_by_uuid", [](asset_registry& self, const uuid& uid) -> const asset_abstract& { return self.find_abstract_by_uuid(uid);}, py::return_value_policy::reference_internal)
		;
	}
}
