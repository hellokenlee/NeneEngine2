/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include "math/dxmath/DirectXMath.h"

using float2 = DirectX::XMFLOAT2;

using float4 = DirectX::XMFLOAT4;

using float4x4 = DirectX::XMFLOAT4X4;

using uint4 = DirectX::XMUINT4;

struct float3 : DirectX::XMFLOAT3
{
    using XMFLOAT3::XMFLOAT3;
    
    static constexpr float3 zero()
    {
        return {};
    }
};
