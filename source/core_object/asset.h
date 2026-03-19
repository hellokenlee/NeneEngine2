/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "py.h"
#include "asset_header.h"
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
		asset();
		virtual ~asset() = default;

		virtual void serialize(archive& ar);
		
		asset_header m_header;
		
		std::string m_test_attrib;
	};
}