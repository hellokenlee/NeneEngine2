/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


/** The type of the reousrce*/
enum class gapi_resource_type
{
	none,
	buffer,
	texture1d,
	texture2d,
	texture3d,
};

/** The usage flag to create a texture */
enum class gapi_texture_create_flag : uint64
{
	none				= 0,
	as_render_target	= 1ull << 0,
	as_shader_resource	= 1ull << 1,
	as_depth_stencil	= 1ull << 2,
	as_unordered_access	= 1ull << 3,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_texture_create_flag);


/** The usage flag to create a buffer */
enum class gapi_buffer_usage_flag : uint64
{
	none				= 0,

	static_buffer			= 1ull << 0,
	dynamic_buffer			= 1ull << 1,
	volatile_buffer			= 1ull << 2,

	shader_resource			= 1ull << 3,

	usage_vertex_buffer		= 1ull << 4,
	usage_index_buffer		= 1ull << 5,
	usage_structured_buffer	= 1ull << 6,
	usage_argument_buffer	= 1ull << 7,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_buffer_usage_flag);


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


/**
*	A resource description is a field how device create resource.
*
*	Equivalents:
*		- DX: ``
*		- VK: ``
*		- MT: ``
*/
struct NENE_API gapi_resource_desc
{
	gapi_resource_type m_type;
	uint32 m_width;
	uint32 m_height;
	uint16 m_depth;
	uint16 m_array_size;
	uint8 m_num_mips;
	uint8 m_num_samples;
	gapi_pixel_format m_format;
	gapi_buffer_usage_flag m_buffer_usage_flag;
	gapi_texture_create_flag m_texture_create_flag;

	gapi_resource_desc(
		gapi_resource_type in_type
		, uint32 in_width
		, uint32 in_height
		, uint16 in_depth
		, uint16 in_array_size
		, uint8 in_num_mips
		, uint8 in_num_samples
		, gapi_pixel_format in_format
		, gapi_buffer_usage_flag in_buffer_flags
		, gapi_texture_create_flag in_texture_flags
	)
	: m_type(in_type)
	, m_width(in_width)
	, m_height(in_height)
	, m_depth(in_depth)
	, m_array_size(in_array_size)
	, m_num_mips(in_num_mips)
	, m_num_samples(in_num_samples)
	, m_format(in_format)
	, m_buffer_usage_flag(in_buffer_flags)
	, m_texture_create_flag(in_texture_flags)
	{
		// TODO: Sanity checks
	}

	static bool is_buffer_desc(const gapi_resource_desc& desc)
	{
		return desc.m_type == gapi_resource_type::buffer;
	}
	
	static bool is_texture_desc(const gapi_resource_desc& desc)
	{
		return desc.m_type == gapi_resource_type::texture1d || desc.m_type == gapi_resource_type::texture2d || desc.m_type == gapi_resource_type::texture3d;
	}
};

/**
 *	Helpers to create a specified resource
 */
namespace gapi_texture_desc
{
	inline gapi_resource_desc create_2d(
		point extent
		, gapi_pixel_format pformat
		, gapi_texture_create_flag flags
		, uint8 num_mips = 1
		, uint8 num_samples = 1
	)
	{
		constexpr uint16 depth = 1;
		constexpr uint16 array_size = 1;
		return gapi_resource_desc(gapi_resource_type::texture2d, extent.x, extent.y, depth, array_size, num_mips, num_samples, pformat, gapi_buffer_usage_flag::shader_resource, flags);
	}
	/*
	inline gapi_resource_desc create_1d()
	{
		;
	}

	inline gapi_resource_desc create_3d()
	{
		;
	}

	inline gapi_resource_desc create_cube()
	{
		;
	}
	*/
}


namespace gapi_buffer_desc
{
	
	inline gapi_resource_desc create(const uint32& width, const gapi_buffer_usage_flag& flags, const uint32& aligment = 0)
	{
		constexpr uint16 height = 1;
		constexpr uint16 depth = 1;
		constexpr uint16 array_size = 1;
		constexpr uint16 num_mips = 1;
		constexpr uint16 num_samples = 1;
		constexpr gapi_pixel_format pformat = gapi_pixel_format::unknown;
		return gapi_resource_desc(gapi_resource_type::texture2d, width, height, depth, array_size, num_mips, num_samples, pformat, flags, gapi_texture_create_flag::none);
	}
}


struct gapi_viewport_desc
{
	vector2 topleft;
	vector2 resolution;
	vector2 depth_range;
};