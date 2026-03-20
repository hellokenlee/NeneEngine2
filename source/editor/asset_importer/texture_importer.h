/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "asset_importer.h"
#include "engine/asset/texture_asset.h"


namespace nene
{
	class texture_importer : public asset_importer
	{
	public:
		std::shared_ptr<g::asset> import_asset(const std::string& from_abs_path) override;
		
		std::vector<std::string> get_supported_asset_extensions() override;
	};
}