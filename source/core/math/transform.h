/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"
#include "quaternion.h"

namespace nene
{
	struct transform
	{
		float3 m_location;
		quaternion m_rotation;
		float3 m_scale;
	};
}