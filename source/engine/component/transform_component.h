/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/mathematics.h"

namespace nene::g
{
	struct local_transform_component
	{
		float3 m_location = float3::zero();
		rotator m_rotation = rotator::zero();
		float3 m_scale = float3::one();
	};
	
	struct world_transform_component
	{
		matrix m_world_matrix = matrix::identity();
	};
}