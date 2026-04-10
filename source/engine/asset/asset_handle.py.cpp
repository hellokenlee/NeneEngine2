/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_handle.h"
#include "asset/static_mesh_asset.h"
#include "asset/material_asset.h"
#include "asset/texture_asset.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::class_<asset_handle_base, py::smart_holder>(m, "AssetHandle")
			.def_readwrite("m_uuid", &static_mesh_asset_handle::m_uuid)
		;
		
		py::class_<static_mesh_asset_handle, asset_handle_base, py::smart_holder>(m, "StaticMeshAssetHandle")
		;
		
		py::class_<material_asset_handle, asset_handle_base, py::smart_holder>(m, "MaterialAssetHandle")
		;
		
		py::class_<texture_asset_handle, asset_handle_base, py::smart_holder>(m, "TextureAssetHandle")
		;
	}
}