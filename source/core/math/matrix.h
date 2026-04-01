/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <DirectXMath.h>
#include "rotator.h"
#include "floats.h"


namespace nene
{
	/** simd accelerated 4x4 float matrix */ 
	struct matrix : DirectX::XMMATRIX
	{
		using DirectX::XMMATRIX::XMMATRIX;
	
		matrix(const DirectX::XMMATRIX& m)
			: XMMATRIX(m)
		{}
	
		// factory methods
		static constexpr matrix identity()
		{
			return {
				DirectX::FXMVECTOR{1.0f, 0.0f, 0.0f, 0.0f},
				DirectX::FXMVECTOR{0.0f, 1.0f, 0.0f, 0.0f},
				DirectX::FXMVECTOR{0.0f, 0.0f, 1.0f, 0.0f},
				DirectX::FXMVECTOR{0.0f, 0.0f, 0.0f, 1.0f},
			};
		}
		
		static matrix make_scale_matrix(const float3& scale)
		{
			return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z); 
		}
		
		static matrix make_rotation_matrix(const rotator& rot)
		{
			return DirectX::XMMatrixRotationRollPitchYaw(rot.pitch, rot.yaw, rot.roll); 
		}
	
		static matrix make_translation_matrix(const float3& trans)
		{
			return DirectX::XMMatrixTranslation(trans.x, trans.y, trans.z); 
		}
		
		static matrix make_view_matrix(const float3& view_location, const float3& view_direction, const float3& up_vector = float3::up_vector())
		{
			const auto vlocation = DirectX::XMLoadFloat3(&view_location);
			const auto vdirection = DirectX::XMLoadFloat3(&view_direction);
			const auto vup = DirectX::XMLoadFloat3(&up_vector);
			return DirectX::XMMatrixLookToLH(vlocation, vdirection, vup);
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
}


/** raw 4x4 float matrix: should only used in cross cpu-gpu data */
struct float4x4 : DirectX::XMFLOAT4X4
{
	using DirectX::XMFLOAT4X4::XMFLOAT4X4;
	
	float4x4& operator=(const nene::matrix& rhs)
	{
		DirectX::XMStoreFloat4x4(this, rhs);
		return *this;
	}
	
	static constexpr float4x4 identity()
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
};
static_assert(sizeof(float4x4) == sizeof(DirectX::XMFLOAT4X4), "cross cpu-gpu data type must have same size!");