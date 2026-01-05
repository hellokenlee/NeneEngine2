/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include "math/dxmath/DirectXMath.h"
#include "math/numeric.h"

using float2 = DirectX::XMFLOAT2;

using float3 = DirectX::XMFLOAT3;

using float4 = DirectX::XMFLOAT4;

using float4x4 = DirectX::XMFLOAT4X4;

using uint2 = numeric2<uint32_t>;

using uint3 = numeric3<uint32_t>;

using uint4 = DirectX::XMUINT4;

/*
 * Other Fundamental Math Types
 */

#include "math/rect.inl"
#include "math/point.inl"

namespace t
{
	template <typename t_class>
	constexpr t_class align(t_class val, uint64 alignment)
	{
		static_assert(std::is_integral_v<t_class>, "align expects an integer type");
		return (t_class)(((uint64)val + alignment - 1) & ~(alignment - 1));
	}	
}
