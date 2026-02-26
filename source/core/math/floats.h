/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/*
 * Simple Proxy For DirectX Math Library
 */

#include <DirectXMath.h>
#include "rotator.h"
#include "constants.h"

struct float2 : DirectX::XMFLOAT2
{
	using XMFLOAT2::XMFLOAT2;
	
	static constexpr float2 zero()
	{
		return {};
	}
	
	bool equals(const float2& rhs, float tolerance = nene::KINDA_SMALL_FLOAT) const
	{
		const auto vlhs = DirectX::XMLoadFloat2(this);
		const auto vrhs = DirectX::XMLoadFloat2(&rhs);
		const auto vabs = DirectX::XMVectorAbs(DirectX::XMVectorSubtract(vlhs, vrhs));
		return DirectX::XMVector2Less(vabs, DirectX::XMLoadFloat(&tolerance));
	}
};
static_assert(sizeof(float2) == sizeof(DirectX::XMFLOAT2), "cross cpu-gpu data type must have same size!");

struct float3 : DirectX::XMFLOAT3
{
	using XMFLOAT3::XMFLOAT3;
	
	// factory methods
	static constexpr float3 zero()
	{
		return {};
	}
	
	static constexpr float3 x_axis_vector()
	{
		return {1.0, 0.0, 0.0};
	}
	
	static constexpr float3 y_axis_vector()
	{
		return {0.0, 1.0, 0.0};
	}
	
	static constexpr float3 z_axis_vector()
	{
		return {0.0, 1.0, 1.0};
	}
	
	/** nene uses left handed coordinate system */
	static constexpr float3 right_vector()
	{
		return x_axis_vector();
	}
	
	/** nene uses left handed coordinate system */
	static constexpr float3 up_vector()
	{
		return y_axis_vector();
	}
	
	/** nene uses left handed coordinate system */
	static constexpr float3 forward_vector()
	{
		return z_axis_vector();
	}
	
	/** construct forward vector of a rotator */
	static float3 make_forward_vector(const nene::rotator& rot)
	{
		// remove winding and clamp to [-360, 360]
		float pitch = fmodf(rot.pitch, 360.0f);
		float yaw = fmodf(rot.yaw, 360.0f);
		// convert ro radians
		pitch = DirectX::XMConvertToRadians(pitch);
		yaw = DirectX::XMConvertToRadians(yaw);
		
		// nene uses left handed coordinate system
		float3 result;
		result.x = cosf(pitch) * sinf(yaw);
		result.y = -sinf(pitch);
		result.z = cosf(pitch) * cosf(yaw);
		return result;
	}
	
	float3 cross(const float3& rhs) const
	{
		const auto vlhs = DirectX::XMLoadFloat3(this);
		const auto vrhs = DirectX::XMLoadFloat3(&rhs);
		const auto vresult = DirectX::XMVector3Cross(vlhs, vrhs);
		float3 result;
		DirectX::XMStoreFloat3(&result, vresult);
		return result;
	}
	
	float3 operator-() const
	{
		return float3{-x, -y, -z };
	}
	
	float3 operator+(const float3& rhs) const
	{
		return float3{x + rhs.x, y + rhs.y, z + rhs.z};
	}
	
	float3 operator+=(const float3& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z; 
		return *this;
	}
	
	float3 operator-(const float3& rhs) const
	{
		return float3{x - rhs.x, y - rhs.y, z - rhs.z};
	}
	
	float3 operator-=(const float3& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z; 
		return *this;
	}
	
	float3 operator*(const float& scalar) const
	{
		return float3{x * scalar, y * scalar, z * scalar};
	}
};
static_assert(sizeof(float3) == sizeof(DirectX::XMFLOAT3), "cross cpu-gpu data type must have same size!");

using float4 = DirectX::XMFLOAT4;
