/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/math.h"


namespace nene::r
{
	struct view_matrices
	{
		float4x4 m_view_matrix;
		float4x4 m_projection_matrix;
		float3 m_view_origin;
	};
}
