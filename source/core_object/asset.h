/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "py.h"
#include "core/uuid.h"
#include "interface/serializable.h"


namespace nene::g
{
	class NENE_API asset : public serializable
	{
	public:
		asset();
		void serialize(archive& ar) override;
		
		uuid m_uuid;
	};
}