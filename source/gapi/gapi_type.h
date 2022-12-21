/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

constexpr uint32 MAX_RENDER_TARGET_COUNT = 8;

enum class gapi_cmd_type
{
	graphics	= 0,
	compute		= 1,
	copy		= 2,
};


enum class gapi_shader_type
{
	vertex_shader			= 0,
	hull_shader				= 1,
	domain_shader			= 2,
	pixel_shader			= 3,
	geometry_shader			= 4,

	compute_shader			= 5,

	ray_gen_shader			= 6,
	ray_miss_shader			= 7,
	ray_hit_group_shader	= 8,
	ray_callable_shader		= 9,
};


enum class gapi_shader_feature_level
{
	sm_5_0,
	sm_5_1,
	sm_6_0,
};

enum class gapi_vertex_element_type
{
	float1,
	float2,
	float3,
	float4,

	half2,
	half4,

	packed_normal,

	unsigned_int,
	unsigned_byte4,
};

enum class gapi_primitive_type
{
	point,
	line,
	triangle,
	patch,
};

enum class gapi_pixel_format
{
	unknown,
	// 8 bits

	// 16 bits
	
	// 32 bits
	r8g8b8a8,
	r8g8b8a8_unorm,

	// 64 bits
	r16g16b16a16,
	r16g16b16a16_unorm,
};
