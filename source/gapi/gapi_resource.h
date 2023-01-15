/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_type.h"
#include "core/flag_enum_class.h"


enum class gapi_resource_type
{
	none,
	buffer,
	texture1d,
	texture2d,
	texture3d,
};

enum class gapi_resource_state
{
	unknown,
	present,
	render_target,
	shader_resource,
};

enum class gapi_texture_create_flag : uint64
{
	none				= 0,
	as_render_target	= 1ull << 0,
	as_shader_resource	= 1ull << 1,
	as_depth_stencil	= 1ull << 2,
	as_unordered_access	= 1ull << 3,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_texture_create_flag);

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


class NENE_API gapi_resource_desc
{
public:
	gapi_resource_desc() = delete;
	gapi_resource_desc(
		gapi_resource_type in_type
		, uint32 in_width
		, uint32 in_height
		, uint16 in_depth
		, uint16 in_array_size
		, uint8 in_num_mips
		, uint8 in_num_samples
		, gapi_pixel_format in_format
		, gapi_texture_create_flag in_flags
	)
	: m_type(in_type)
	, m_width(in_width)
	, m_height(in_height)
	, m_depth(in_depth)
	, m_array_size(in_array_size)
	, m_num_mips(in_num_mips)
	, m_num_samples(in_num_samples)
	, m_format(in_format)
	, m_texture_create_flag(in_flags)
	{}
	
	gapi_resource_type m_type;
	uint32 m_width;
	uint32 m_height;
	uint16 m_depth;
	uint16 m_array_size;
	uint8 m_num_mips;
	uint8 m_num_samples;
	gapi_pixel_format m_format;
	gapi_texture_create_flag m_texture_create_flag;
};

class NENE_API gapi_texture_desc : public gapi_resource_desc
{
public:
	static gapi_texture_desc create_2d(
		point extent
		, gapi_pixel_format pformat
		, gapi_texture_create_flag flags
		, uint8 num_mips = 1
		, uint8 num_samples = 1
	)
	{
		constexpr uint16 depth = 1;
		constexpr uint16 array_size = 1;
		return gapi_texture_desc(gapi_resource_type::texture2d, extent.x, extent.y, depth, array_size, num_mips, num_samples, pformat, flags);
	}

	using gapi_resource_desc::gapi_resource_desc;
};

class gapi_buffer
{
public:
	virtual ~gapi_buffer() = default;
};
