/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include "math/dxmath/DirectXMath.h"
#include "rotator.h"

using float2 = DirectX::XMFLOAT2;

using float4 = DirectX::XMFLOAT4;

using uint4 = DirectX::XMUINT4;

struct float3 : DirectX::XMFLOAT3
{
    using XMFLOAT3::XMFLOAT3;
    
    static constexpr float3 zero()
    {
        return {};
    }
    
    float3 operator-() const
    {
        return float3{-x, -y, -z };
    }
};

/** simd accelerated 4x4 float matrix */ 
struct matrix : DirectX::XMMATRIX
{
    using DirectX::XMMATRIX::XMMATRIX;
    
    /** create a rotation matrix */
    matrix(const rotator& rot)
        : DirectX::XMMATRIX(DirectX::XMMatrixRotationRollPitchYaw(rot.pitch, rot.yaw, rot.roll))
    {}
    
    matrix(const float3& trans)
        : DirectX::XMMATRIX(DirectX::XMMatrixTranslation(trans.x, trans.y, trans.z))
    {}
    
    matrix transpose() const
    {
        DirectX::XMMATRIX m = DirectX::XMMatrixTranspose(*this);
        return static_cast<matrix&>(m);
    }
    
    matrix operator*(const matrix& other) const
    {
        DirectX::XMMATRIX m = (DirectX::XMMatrixMultiply(*this, other));
        return static_cast<matrix&>(m);
    }
};

/** raw 4x4 float matrix: should only used in cross cpu-gpu data */
struct float4x4 : DirectX::XMFLOAT4X4
{
    float4x4& operator=(const matrix& rhs)
    {
        DirectX::XMStoreFloat4x4(this, rhs);
        return *this;
    }
};
