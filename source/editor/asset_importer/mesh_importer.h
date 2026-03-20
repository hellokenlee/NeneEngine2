/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "asset_importer.h"
#include "core/core.h"


namespace nene
{
	class mesh_importer : public asset_importer
	{
	public:
		std::shared_ptr<g::asset> import_asset(const std::string& from_abs_path) override;
		
		std::vector<std::string> get_supported_asset_extensions() override;
	};
}
