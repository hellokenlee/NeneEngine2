/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_pipeline_layout.h"
#include "gapi_shader.h"
#include "gapi_resource_desc.h"


constexpr uint32 MAX_RENDER_TARGET_COUNT = 8;

/**
 *	Bound Shader State
 */
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

struct gapi_vertex_element_desc
{
	sstring semantic_name;
	uint8 attrib_index;
	gapi_vertex_element_type type;
	uint8 stream_index;
	uint8 offset;
	uint8 use_instance_index;
	uint16 stride;
};
typedef t::dynamic_array<gapi_vertex_element_desc> gapi_vertex_declartions;

enum class gapi_primitive_type
{
	point,
	line,
	triangle,
	patch,
};

struct NENE_API gapi_bound_shader_desc
{
	//
	t::dynamic_array<gapi_vertex_element_desc> m_vertex_declaration;
	//
	t::shared_ptr<i::gapi_vertex_shader> m_vertex_shader;
	t::shared_ptr<i::gapi_pixel_shader> m_pixel_shader;
	t::shared_ptr<i::gapi_domain_shader> m_domain_shader;
	t::shared_ptr<i::gapi_hull_shader> m_hull_shader;
	t::shared_ptr<i::gapi_geometry_shader> m_geometry_shader;
	//
	t::shared_ptr<i::gapi_compute_shader> m_compute_shader;
	//
	t::shared_ptr<i::gapi_mesh_shader> m_mesh_shader;
	t::shared_ptr<i::gapi_amplification_shader> m_amplification_shader;
	//
	t::shared_ptr<i::gapi_ray_gen_shader> m_ray_gen_shader;
	t::shared_ptr<i::gapi_ray_tracing_shader> m_ray_tracing_shader;

	// Graphic Shader Stages
	gapi_bound_shader_desc(
		const gapi_vertex_declartions& vertex_declaration,
		const t::shared_ptr<i::gapi_vertex_shader>& vertex_shader,
		const t::shared_ptr<i::gapi_pixel_shader>& pixel_shader = {},
		const t::shared_ptr<i::gapi_domain_shader>& domain_shader = {},
		const t::shared_ptr<i::gapi_hull_shader>& hull_shader = {},
		const t::shared_ptr<i::gapi_geometry_shader>& geometry_shader = {}
	);

	// Compute Shader Stages
	gapi_bound_shader_desc(
		const t::shared_ptr<i::gapi_compute_shader>& compute_shader
	);

private:
	gapi_bound_shader_desc(
		const t::dynamic_array<gapi_vertex_element_desc>& in_vertex_declaration,
		const t::shared_ptr<i::gapi_vertex_shader>& in_vertex_shader,
		const t::shared_ptr<i::gapi_pixel_shader>& in_pixel_shader,
		const t::shared_ptr<i::gapi_domain_shader>& in_domain_shader,
		const t::shared_ptr<i::gapi_hull_shader>& in_hull_shader,
		const t::shared_ptr<i::gapi_geometry_shader>& in_geometry_shader,
		const t::shared_ptr<i::gapi_compute_shader>& in_compute_shader,
		const t::shared_ptr<i::gapi_mesh_shader>& in_mesh_shader,
		const t::shared_ptr<i::gapi_amplification_shader>& in_amplification_shader,
		const t::shared_ptr<i::gapi_ray_gen_shader>& in_ray_gen_shader,
		const t::shared_ptr<i::gapi_ray_tracing_shader>& in_ray_tracing_shader
	)
		: m_vertex_declaration(in_vertex_declaration)
		, m_vertex_shader(in_vertex_shader)
		, m_pixel_shader(in_pixel_shader)
		, m_domain_shader(in_domain_shader)
		, m_hull_shader(in_hull_shader)
		, m_geometry_shader(in_geometry_shader)
		, m_compute_shader(in_compute_shader)
		, m_mesh_shader(in_mesh_shader)
		, m_amplification_shader(in_amplification_shader)
		, m_ray_gen_shader(in_ray_gen_shader)
		, m_ray_tracing_shader(in_ray_tracing_shader)
	{}
};


/**
 *	Blend State
 */
enum class gapi_blend_op
{
	add,
	substract,
	min,
	max,
	reverse_substract,
};

enum class gapi_blend_factor
{
	zero,
	one,
	source_color,
	inverse_source_color,
	source_alpha,
	inver_sesource_alpha,
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

enum class gapi_color_write_mask
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
		
		gapi_render_target_blend_desc(
			const gapi_blend_op& in_color_blend_op = gapi_blend_op::add,
			const gapi_blend_factor& in_color_src_blend = gapi_blend_factor::one,
			const gapi_blend_factor& in_color_dest_blend = gapi_blend_factor::zero,
			const gapi_blend_op& in_alpha_blend_op = gapi_blend_op::add,
			const gapi_blend_factor& in_alpha_src_blend = gapi_blend_factor::one,
			const gapi_blend_factor& in_alpha_dest_blend = gapi_blend_factor::zero,
			const gapi_color_write_mask& in_color_write_mask = gapi_color_write_mask::rgba
		)
			: color_blend_op(in_color_blend_op)
			, color_src_blend(in_color_src_blend)
			, color_dest_blend(in_color_dest_blend)
			, alpha_blend_op(in_alpha_blend_op)
			, alpha_src_blend(in_alpha_src_blend)
			, alpha_dest_blend(in_alpha_dest_blend)
			, color_write_mask(in_color_write_mask)
		{}
	};

	gapi_blend_state_desc() = delete;

	gapi_blend_state_desc(const gapi_render_target_blend_desc& in_render_target_blend_desc, bool in_use_alpha_to_coverage = false)
		: m_use_alpha_to_coverage(in_use_alpha_to_coverage)
	{
		m_render_target_blend_descs[0] = in_render_target_blend_desc;
	}

	template<uint32 num_render_targets>
	gapi_blend_state_desc(const t::static_array<gapi_render_target_blend_desc, num_render_targets>& in_render_target_blend_descs, bool in_use_alpha_to_coverage)
		: m_use_alpha_to_coverage(in_use_alpha_to_coverage)
		, m_use_independent_blend(num_render_targets > 1)
	{
		static_assert(num_render_targets <= MAX_RENDER_TARGET_COUNT, "Too many render target in blend states.");
		m_render_target_blend_descs = in_render_target_blend_descs;
	}

	bool m_use_alpha_to_coverage = false;
	bool m_use_independent_blend = false;
	t::static_array<gapi_render_target_blend_desc, MAX_RENDER_TARGET_COUNT> m_render_target_blend_descs;
};


/**
 *	Rasterizer State
 */
enum class gapi_rasterizer_fill_mode
{
	point,
	wireframe,
	solid,
};

enum class gapi_rasterizer_cull_mode
{
	none,
	cw,
	ccw,
};

enum class gapi_rasterizer_depth_clip_mode
{
	clip,
	clamp,
};

struct gapi_rasterizer_state_desc
{
	gapi_rasterizer_fill_mode m_fill_mode;
	gapi_rasterizer_cull_mode m_cull_mode;
	gapi_rasterizer_depth_clip_mode m_depth_clip_mode;
	float m_depth_bias;
	float m_sclop_scale_depth_bias;
	bool m_use_msaa;
	bool m_use_line_aa;

	gapi_rasterizer_state_desc(
		const gapi_rasterizer_fill_mode& in_fill_mode = gapi_rasterizer_fill_mode::point,
		const gapi_rasterizer_cull_mode& in_cull_mode = gapi_rasterizer_cull_mode::none,
		const gapi_rasterizer_depth_clip_mode& in_depth_clip_mode =gapi_rasterizer_depth_clip_mode::clip,
		const float& in_depth_bias = 0.0f,
		const float& in_sclop_scale_depth_bias = 0.0f,
		const bool& in_use_msaa = false,
		const bool& in_use_line_aa = false
	)
		: m_fill_mode(in_fill_mode)
		, m_cull_mode(in_cull_mode)
		, m_depth_clip_mode(in_depth_clip_mode)
		, m_depth_bias(in_depth_bias)
		, m_sclop_scale_depth_bias(in_sclop_scale_depth_bias)
		, m_use_msaa(in_use_msaa)
		, m_use_line_aa(in_use_line_aa)
	{}
};


/**
 *	Depth Stencil State
 */
enum class gapi_cmp_func
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

enum class gapi_stencil_op
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

	// Stenicl
	struct gapi_stencil_state_desc
	{
		bool m_use_stencil;
		gapi_cmp_func m_stencil_func;
		gapi_stencil_op m_stencil_fail_op;		// Stencil: x; Depth: -;
		gapi_stencil_op m_depth_fail_op;		// Stencil: v; Depth: x;
		gapi_stencil_op m_pass_op;				// Stencil: v; Depth: v;
	};
	gapi_stencil_state_desc m_front_face_stencil_test;
	gapi_stencil_state_desc m_back_face_stenci_test;
	uint8 m_stencil_read_mask;
	uint8 m_stencil_write_mask;

	gapi_depth_stencil_state_desc(
		const bool& in_use_depth_write = true,
		const gapi_cmp_func& in_depth_func = gapi_cmp_func::less_equal,
		const gapi_stencil_state_desc& in_front_face_stencil_test = {.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep},
		const gapi_stencil_state_desc& in_back_face_stencil_test  = {.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep},
		const uint8& in_stencil_read_mask = 0x00,
		const uint8& in_stencil_write_mask = 0x00
	);
};

/**
 *	The argument that needed to create a pipeline state object.
 */
struct NENE_API gapi_graphics_pipeline_state_desc
{
	gapi_bound_shader_desc m_bound_shader_state;
	gapi_blend_state_desc m_blend_state;
	gapi_rasterizer_state_desc m_rasterizer_state;
	gapi_depth_stencil_state_desc m_depth_stencil_state;
	gapi_primitive_type m_primitive_type;
	uint16 m_num_samples;
	gapi_pixel_format m_depth_stencil_format;
	t::dynamic_array<gapi_pixel_format> m_render_target_formats;
	t::shared_ptr<i::gapi_pipeline_layout> m_pipeline_layout; 

	gapi_graphics_pipeline_state_desc(
		const gapi_bound_shader_desc& bound_shader_state,
		const gapi_blend_state_desc& blend_state,
		const gapi_rasterizer_state_desc& rasterizer_state,
		const gapi_depth_stencil_state_desc& depth_stencil_state,
		const gapi_primitive_type& primitive_type,
		const uint16& num_samples,
		const gapi_pixel_format& depth_stencil_format,
		const t::dynamic_array<gapi_pixel_format>& render_target_formats,
		const t::shared_ptr<i::gapi_pipeline_layout>& pipeline_layout = nullptr
	)
		: m_bound_shader_state(bound_shader_state)
		, m_blend_state(blend_state)
		, m_rasterizer_state(rasterizer_state)
		, m_depth_stencil_state(depth_stencil_state)
		, m_primitive_type(primitive_type)
		, m_num_samples(num_samples)
		, m_depth_stencil_format(depth_stencil_format)
		, m_render_target_formats(render_target_formats)
		, m_pipeline_layout(pipeline_layout)
	{
		CHECK(render_target_formats.size() < MAX_RENDER_TARGET_COUNT);
	}
	
	/*
	FImmutableSamplerState			ImmutableSamplerState;

	FExclusiveDepthStencil			DepthStencilAccess;
	ESubpassHint					SubpassHint;
	uint8							SubpassIndex;
	EConservativeRasterization		ConservativeRasterization;
	bool							bDepthBounds;
	uint8							MultiViewCount;
	bool							bHasFragmentDensityAttachment;
	EVRSShadingRate					ShadingRate;

	uint64							StatePrecachePSOHash;
	*/
};


struct NENE_API gapi_compute_pipeline_state_desc
{
	gapi_bound_shader_desc m_bound_shader_state;

	gapi_compute_pipeline_state_desc(
		const gapi_bound_shader_desc& bound_shader_state
	)
		: m_bound_shader_state(bound_shader_state)
	{}
};