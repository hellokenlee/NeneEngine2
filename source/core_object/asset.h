/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "py.h"
#include "asset_abstract.h"
#include "core/uuid.h"
#include "core/windll.h"
#include "archive/archive.h"


namespace nene::g
{
	/**
	 *	base class for serialization
	 */
	class NENE_API asset
	{
	public:
		virtual ~asset() = default;

		virtual void serialize(archive& ar);
		
		uuid m_uuid = {};
		
	protected:
		// dont use `std::make_shared<asset_t>` directly
		// use `asset_importer::make_asset<asset_t>` instead
		asset();
		friend class asset_importer;
	};
}