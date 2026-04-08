/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_handle.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::class_<asset_handle_base>(m, "AssetHandle")
			.def_readwrite("m_uuid", &static_mesh_asset_handle::m_uuid)
		;
		
		py::class_<static_mesh_asset_handle, asset_handle_base>(m, "StaticMeshAssetHandle")
		;
		
		py::class_<material_asset_handle, asset_handle_base>(m, "MaterialAssetHandle")
		;
	}
}