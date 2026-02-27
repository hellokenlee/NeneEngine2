/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/math.h"

namespace nene::g
{
	struct transform_component
	{
		float3 m_location = float3::zero();
		float3 m_rotation = float3::zero();
		float3 m_scale = float3::one();
	};
}