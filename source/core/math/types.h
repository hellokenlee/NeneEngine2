/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include <DirectXMath.h>
#include "rotator.h"

constexpr float KINDA_SMALL_FLOAT = 1.e-4f;

using float4 = DirectX::XMFLOAT4;

using uint4 = DirectX::XMUINT4;

struct float2 : DirectX::XMFLOAT2
{
	using XMFLOAT2::XMFLOAT2;
	
	static constexpr float2 zero()
	{
		return {};
	}
	
	bool equals(const float2& rhs, float tolerance = KINDA_SMALL_FLOAT) const
	{
		const auto vlhs = DirectX::XMLoadFloat2(this);
		const auto vrhs = DirectX::XMLoadFloat2(&rhs);
		const auto vabs = DirectX::XMVectorAbs(DirectX::XMVectorSubtract(vlhs, vrhs));
		return DirectX::XMVector2Less(vabs, DirectX::XMLoadFloat(&tolerance));
	}
};

struct float3 : DirectX::XMFLOAT3
{
	using XMFLOAT3::XMFLOAT3;
	
	static constexpr float3 zero()
	{
		return {};
	}
	
	/** construct forward vector of a rotator */
	float3(const nene::rotator& rot)
	{
		// remove winding and clamp to [-360, 360]
		float pitch = fmodf(rot.pitch, 360.0f);
		float yaw = fmodf(rot.yaw, 360.0f);
		// convert ro radians
		pitch = DirectX::XMConvertToRadians(pitch);
		yaw = DirectX::XMConvertToRadians(yaw);
		
		// nene uses left handed coordinate system
		x = cosf(pitch) * sinf(yaw);
		y = -sinf(pitch);
		z = cosf(pitch) * cosf(yaw);
	}
	
	float3 operator-() const
	{
		return float3{-x, -y, -z };
	}
	
	float3 operator+(const float3& rhs) const
	{
		return float3{x + rhs.x, y + rhs.y, z + rhs.z};
	}
	
	float3 operator-(const float3& rhs) const
	{
		return float3{x - rhs.x, y - rhs.y, z - rhs.z};
	}
	
	float3 operator*(const float& scalar) const
	{
		return float3{x * scalar, y * scalar, z * scalar};
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
