/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


/** The type of the resource */
enum class gapi_resource_type : uint8_t
{
	none,
	buffer,
	texture1d,
	texture2d,
	texture3d,
};

/** The usage flag to create a texture */
enum class gapi_texture_create_flag : uint8_t
{
	none				= 0,
	// texture can be used as a render target
	as_render_target	= 1ull << 0,
	// texture can be used as a shader resource
	as_shader_resource	= 1ull << 1,
	// texture can be used as a depth-stencil target
	as_depth_stencil	= 1ull << 2,
	// texture can be used through an unordered access view
	as_unordered_access	= 1ull << 3,

	// render target texture that will be displayed on screen (back buffer)
	presentable			= 1ull << 4,

	// texture data is writable by the CPU
	cpu_writable		= 1ull << 5,
	// texture data is accessible by the CPU
	cpu_readable		= 1ull << 6,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_texture_create_flag);


/** The usage flag to create a buffer */
enum class gapi_buffer_usage_flag : uint16_t
{
	none					= 0,

	// the buffer would not change after creation
	static_buffer			= 1ull << 0,
	// the buffer is cpu write only, gpu read only
	dynamic_buffer			= 1ull << 1,
	// the buffer is only valid for 1 frame
	volatile_buffer			= 1ull << 2,

	// the buffer can be bound as a shader resource
	shader_resource			= 1ull << 3,
	constant_buffer			= 1ull << 4,
	unordered_access		= 1ull << 5,

	// the buffer is created as a vertex buffer
	usage_vertex_buffer		= 1ull << 6,
	// the buffer is created as an index buffer
	usage_index_buffer		= 1ull << 7,
	// the buffer is created as a structured buffer
	usage_structured_buffer	= 1ull << 8,
	// the buffer is created as an argument buffer for indirect draw
	usage_argument_buffer	= 1ull << 9,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_buffer_usage_flag);


enum class gapi_pixel_format : uint8_t
{
	unknown,
	// 8 bits

	// 16 bits
	
	// 32 bits
	r8g8b8a8,
	r8g8b8a8_unorm,
	d24_s8,

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
	uint64_t m_width;
	uint32_t m_height;
	uint16_t m_depth;
	uint16_t m_array_size;
	uint8_t m_num_mips;
	uint8_t m_num_samples;
	uint16_t m_vertex_buffer_stride;
	gapi_pixel_format m_format;
	gapi_buffer_usage_flag m_buffer_usage_flag;
	gapi_texture_create_flag m_texture_create_flag;
	std::string m_debug_name;

	uint64_t buffer_size() const
	{
		return m_width;
	}
	
	bool is_buffer() const
	{
		return m_type == gapi_resource_type::buffer;
	}
	
	bool is_texture() const
	{
		return m_type == gapi_resource_type::texture1d || m_type == gapi_resource_type::texture2d || m_type == gapi_resource_type::texture3d;
	}
};

/**
 *	Helpers to create a specified resource
 */
namespace gapi_texture_desc
{
	inline gapi_resource_desc create_1d(uint32_t extent, gapi_pixel_format pformat, gapi_texture_create_flag flags, uint8_t num_mips = 1, const std::string& debug_name = "UnnamedTexture1D")
	{
		return gapi_resource_desc{
			.m_type = gapi_resource_type::texture1d,
			.m_width = extent,
			.m_height = 1,
			.m_depth = 1,
			.m_array_size = 1,
			.m_num_mips = 1,
			.m_num_samples = 1,
			.m_format = pformat,
			.m_buffer_usage_flag = gapi_buffer_usage_flag::none,
			.m_texture_create_flag = flags,
			.m_debug_name = debug_name,
		};
	}
	
	inline gapi_resource_desc create_2d(uint2 extent, gapi_pixel_format pformat, gapi_texture_create_flag flags, uint8_t num_mips = 1, uint8_t num_samples = 1, const std::string& debug_name = "UnnamedTexture2D")
	{
		return gapi_resource_desc{
			.m_type = gapi_resource_type::texture2d,
			.m_width = extent.x,
			.m_height = extent.y,
			.m_depth = 1,
			.m_array_size = 1,
			.m_num_mips = num_mips,
			.m_num_samples = num_samples,
			.m_format = pformat,
			.m_buffer_usage_flag = gapi_buffer_usage_flag::none,
			.m_texture_create_flag = flags,
			.m_debug_name = debug_name,
		};
	}
	
	inline gapi_resource_desc create_3d(uint3 extent, gapi_pixel_format pformat, gapi_texture_create_flag flags, uint8_t num_mips = 1, const std::string& debug_name = "UnnamedTexture1D")
	{
		return gapi_resource_desc{
			.m_type = gapi_resource_type::texture2d,
			.m_width = extent.x,
			.m_height = extent.y,
			.m_depth = static_cast<uint16_t>(extent.z),
			.m_array_size = 1,
			.m_num_mips = 1,
			.m_num_samples = 1,
			.m_format = pformat,
			.m_buffer_usage_flag = gapi_buffer_usage_flag::none,
			.m_texture_create_flag = flags,
			.m_debug_name = debug_name,
		};
	}
}


namespace gapi_buffer_desc
{
	inline gapi_resource_desc create(uint64_t size, const gapi_buffer_usage_flag& flags, uint16_t stride = 0, const std::string& debug_name= "UnnamedBuffer")
	{
		return gapi_resource_desc{
			.m_type = gapi_resource_type::buffer,
			.m_width = size,
			.m_height = 1,
			.m_depth = 1,
			.m_array_size = 1,
			.m_num_mips = 1,
			.m_num_samples = 1,
			.m_vertex_buffer_stride = stride,
			.m_format = gapi_pixel_format::unknown,
			.m_buffer_usage_flag = flags,
			.m_texture_create_flag = gapi_texture_create_flag::none,
			.m_debug_name = debug_name,
		};
	}
}


struct gapi_viewport_desc
{
	float2 top_left;
	float2 resolution;
	float2 depth_range;
};