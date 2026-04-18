/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_handle.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::classh<asset_handle_base>(m, "AssetHandle")
			.def_readwrite("m_uuid", &static_mesh_asset_handle::m_uuid)
		;
		
		py::classh<static_mesh_asset_handle, asset_handle_base>(m, "StaticMeshAssetHandle")
			.def(py::init<>())
		;
		
		py::classh<material_asset_handle, asset_handle_base>(m, "MaterialAssetHandle")
			.def(py::init<>())
		;
		
		py::classh<texture_asset_handle, asset_handle_base>(m, "TextureAssetHandle")
			.def(py::init<>())
		;
	}
}