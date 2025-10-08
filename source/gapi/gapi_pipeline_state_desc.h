/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_bound_shader_signature.h"
#include "gapi_shader.h"
#include "gapi_resource_desc.h"


constexpr uint32 MAX_RENDER_TARGET_COUNT = 8;

/**
 *	Bound Shader State
 */
enum class gapi_vertex_element_type : uint8
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

struct gapi_vertex_element_desc
{
	std::string semantic_name;
	uint8 attribute_index;
	gapi_vertex_element_type element_type;
	uint8 stream_index;
	uint8 offset;
	// 
	bool b_use_instance_index : 1;
	uint16 stride;
};
typedef std::vector<gapi_vertex_element_desc> gapi_vertex_declaration;

enum class gapi_primitive_type : uint8
{
	point,
	line,
	triangle,
	patch,
};

struct NENE_API gapi_bound_shader_state_desc
{
	// 
	std::shared_ptr<gapi_vertex_declaration> m_vertex_declaration;
	//
	std::array<std::shared_ptr<i::gapi_shader>, magic_enum::enum_count<gapi_shader_type>()> m_stage_shaders;

	//
	template<gapi_shader_type stage>
	const std::shared_ptr<i::gapi_shader>& get_stage_shader() const
	{
		return m_stage_shaders[magic_enum::enum_underlying(stage)];
	}

	// Graphic Shader Stages
	gapi_bound_shader_state_desc(const std::shared_ptr<gapi_vertex_declaration>& vertex_declaration, const std::shared_ptr<i::gapi_shader>& vertex_shader);
	gapi_bound_shader_state_desc(const std::shared_ptr<gapi_vertex_declaration>& vertex_declaration, const std::shared_ptr<i::gapi_shader>& vertex_shader, const std::shared_ptr<i::gapi_shader>& pixel_shader);

	// Compute Shader Stages
	gapi_bound_shader_state_desc(const std::shared_ptr<i::gapi_shader>& compute_shader);
	
	// No default constructor
	gapi_bound_shader_state_desc() = delete;

private:
	void sanity_check() const;
};


/**
 *	Blend State
 */
enum class gapi_blend_op: uint8
{
	add,
	subtract,
	min,
	max,
	reverse_subtract,
};

enum class gapi_blend_factor: uint8
{
	zero,
	one,
	source_color,
	inverse_source_color,
	source_alpha,
	invert_source_alpha,
	dest_alpha,
	inverse_dest_alpha,
	dest_color,
	inverse_dest_color,
	constant_blend_factor,
	inverse_constant_blend_factor,
	source1_color,
	inverse_source1_color,
	source1_alpha,
	inverse_source1_alpha,
};

enum class gapi_color_write_mask: uint8
{
	//
	none = 0x00,
	//
	r = 0x01,
	g = 0x02,
	b = 0x04,
	a = 0x08,
	//
	rgb   = r | g | b,
	rgba  = r | g | b | a,
	rg    = r | g,
	ba    = b | a,
};
DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_color_write_mask)

struct gapi_blend_state_desc
{
	struct gapi_render_target_blend_desc
	{
		gapi_blend_op color_blend_op;
		gapi_blend_factor color_src_blend;
		gapi_blend_factor color_dest_blend;
		gapi_blend_op alpha_blend_op;
		gapi_blend_factor alpha_src_blend;
		gapi_blend_factor alpha_dest_blend;
		gapi_color_write_mask color_write_mask;
		
		gapi_render_target_blend_desc();
	};

	gapi_blend_state_desc() = default;

	gapi_blend_state_desc(const gapi_render_target_blend_desc& in_render_target_blend_desc, bool in_use_alpha_to_coverage = false)
		: m_use_alpha_to_coverage(in_use_alpha_to_coverage)
	{
		m_render_target_blend_descs[0] = in_render_target_blend_desc;
	}

	template<uint32 num_render_targets>
	gapi_blend_state_desc(const std::array<gapi_render_target_blend_desc, num_render_targets>& in_render_target_blend_descs, bool in_use_alpha_to_coverage)
		: m_use_alpha_to_coverage(in_use_alpha_to_coverage)
		, m_use_independent_blend(num_render_targets > 1)
	{
		static_assert(num_render_targets <= MAX_RENDER_TARGET_COUNT, "Too many render target in blend states.");
		m_render_target_blend_descs = in_render_target_blend_descs;
	}

	bool m_use_alpha_to_coverage = false;
	bool m_use_independent_blend = false;
	std::array<gapi_render_target_blend_desc, MAX_RENDER_TARGET_COUNT> m_render_target_blend_descs;
};


/**
 *	Rasterizer State
 */
enum class gapi_rasterizer_fill_mode: uint8
{
	wireframe,
	solid,
};

enum class gapi_rasterizer_cull_mode : uint8
{
	// dont cull anything
	cull_none,
	// cull back face ( clock wise )
	cull_back,
	// cull front face ( counter clock wise )
	cull_front,
};

enum class gapi_rasterizer_depth_clip_mode : uint8
{
	// discard the pixel if its depth is out of range
	clip,
	// change the pixel's depth to extremum if its depth is out of range
	clamp,
};

struct gapi_rasterizer_state_desc
{
	gapi_rasterizer_fill_mode m_fill_mode;
	gapi_rasterizer_cull_mode m_cull_mode;
	gapi_rasterizer_depth_clip_mode m_depth_clip_mode;
	float m_depth_bias;
	float m_slope_scale_depth_bias;
	bool m_use_msaa;
	bool m_use_line_aa;

	// Default Constructor
	gapi_rasterizer_state_desc();
};


/**
 *	Depth Stencil State
 */
enum class gapi_cmp_func : uint8
{
	less,
	less_equal,
	greater,
	greater_equal,
	equal,
	not_equal,
	never,
	always,
};

enum class gapi_stencil_op : uint8
{
	keep,
	zero,
	replace,
	saturated_increment,
	saturated_decrement,
	invert,
	increment,
	decrement,
};

struct gapi_depth_stencil_state_desc
{
	// Depth
	bool m_use_depth_write;
	gapi_cmp_func m_depth_func;

	// Stencil
	struct gapi_stencil_state_desc
	{
		bool m_use_stencil;
		gapi_cmp_func m_stencil_func;
		gapi_stencil_op m_stencil_fail_op;		// Stencil: x; Depth: -;
		gapi_stencil_op m_depth_fail_op;		// Stencil: v; Depth: x;
		gapi_stencil_op m_pass_op;				// Stencil: v; Depth: v;
	};
	gapi_stencil_state_desc m_front_stencil_test;
	gapi_stencil_state_desc m_back_stencil_test;
	uint8 m_stencil_read_mask;
	uint8 m_stencil_write_mask;

	// Default Constructor
	gapi_depth_stencil_state_desc();
};

/**
 *	The argument that needed to create a pipeline state object.
 */
struct NENE_API gapi_compute_pipeline_state_desc
{
	gapi_bound_shader_state_desc m_bound_shader_state;

	gapi_compute_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state);
};

struct NENE_API gapi_graphics_pipeline_state_desc
{
	// shaders
	gapi_bound_shader_state_desc m_bound_shader_state;
	// RS
	gapi_rasterizer_state_desc m_rasterizer_state;
	gapi_primitive_type m_primitive_type = gapi_primitive_type::triangle;
	// OM
	uint16 m_num_samples = 1;
	gapi_blend_state_desc m_blend_state;
	gapi_depth_stencil_state_desc m_depth_stencil_state;
	gapi_pixel_format m_depth_stencil_format = gapi_pixel_format::unknown;
	std::vector<gapi_pixel_format> m_render_target_formats;

	// Minimal Constructor
	gapi_graphics_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state);
};
