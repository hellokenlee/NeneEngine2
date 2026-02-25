/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include <DirectXMath.h>
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
	
	matrix(const DirectX::XMMATRIX& m)
		: XMMATRIX(m)
	{}
	
	/** factory methods */
	static matrix make_rotation_matrix(const nene::rotator& rot)
	{
		return DirectX::XMMatrixRotationRollPitchYaw(rot.pitch, rot.yaw, rot.roll); 
	}
	
	static matrix make_translation_matrix(const float3& trans)
	{
		return DirectX::XMMatrixTranslation(trans.x, trans.y, trans.z); 
	}
	
	static matrix make_perspective_projection_matrix(float fov, float ratio, float near_plane, float far_plane)
	{
		return DirectX::XMMatrixPerspectiveFovLH(fov, ratio, near_plane, far_plane);
	}
	
	static matrix make_orthographic_projection_matrix(float width, float height, float near_plane, float far_plane)
	{
		return DirectX::XMMatrixOrthographicLH(width, height, near_plane, far_plane);
	}
	
	matrix transpose() const
	{
		return DirectX::XMMatrixTranspose(*this);
	}
	
	matrix operator*(const matrix& other) const
	{
		return DirectX::XMMatrixMultiply(*this, other);
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
