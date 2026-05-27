/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_shader.h"
#include "gapi_resource_desc.h"


namespace nene
{
	constexpr uint32_t MAX_RENDER_TARGET_COUNT = 8;

	/**
	 *	Bound Shader State ( name = stride )
	 */
	enum class gapi_vertex_element_type : uint8_t
	{
		float1 = 0,
		float2,
		float3,
		float4,

		half2,
		half4,

		unsigned_int,
		unsigned_byte4,
	};
	NENE_API uint16_t size_of_gapi_vertex_element_type(const gapi_vertex_element_type& element_type);

	struct gapi_vertex_element_desc
	{
		std::string m_semantic_name;
		uint8_t m_semantic_index;
		gapi_vertex_element_type m_element_type;
		uint8_t m_stream_index;
		uint8_t m_offset;
		// 
		bool m_use_instance_index : 1;
		uint16_t m_stride;

		bool operator==(const gapi_vertex_element_desc& other) const
		{
			return m_semantic_name == other.m_semantic_name &&
				   m_semantic_index == other.m_semantic_index &&
				   m_element_type == other.m_element_type &&
				   m_stream_index == other.m_stream_index &&
				   m_offset == other.m_offset &&
				   m_use_instance_index == other.m_use_instance_index &&
				   m_stride == other.m_stride;
		}
	};
	typedef std::vector<gapi_vertex_element_desc> gapi_vertices_declaration;

	enum class gapi_primitive_type : uint8_t
	{
		point,
		line,
		triangle,
		patch,
	};

	class NENE_API gapi_bound_shader_state_desc
	{
	public:
		// No default constructor
		gapi_bound_shader_state_desc() = delete;
		// Graphic Shader Stages
		gapi_bound_shader_state_desc(const gapi_vertices_declaration& vertex_declaration, const std::shared_ptr<gapi_shader>& vertex_shader);
		gapi_bound_shader_state_desc(const gapi_vertices_declaration& vertex_declaration, const std::shared_ptr<gapi_shader>& vertex_shader, const std::shared_ptr<gapi_shader>& pixel_shader);
		// Compute Shader Stages
		gapi_bound_shader_state_desc(const std::shared_ptr<gapi_shader>& compute_shader);

		//
		const std::shared_ptr<gapi_shader>& get_shader(gapi_shader_stage stage) const
		{
			return m_stage_shaders[magic_enum::enum_underlying(stage)];
		}
		void set_shader(gapi_shader_stage stage, const std::shared_ptr<gapi_shader>& shader)
		{
			m_stage_shaders[magic_enum::enum_underlying(stage)] = shader;
		}
		//
		const auto& get_vertices_declaration() const { return m_vertices_declaration; }

		bool operator==(const gapi_bound_shader_state_desc& other) const
		{
			return m_vertices_declaration == other.m_vertices_declaration && m_stage_shaders == other.m_stage_shaders;
		}

	protected:
		void sanity_check() const;
		// 
		gapi_vertices_declaration m_vertices_declaration;
		//
		std::array<std::shared_ptr<gapi_shader>, NUM_GAPI_SHADER_STAGE> m_stage_shaders;
		
	};


	/**
	 *	Blend State
	 */
	enum class gapi_blend_op: uint8_t
	{
		add,
		subtract,
		min,
		max,
		reverse_subtract,
	};

	enum class gapi_blend_factor: uint8_t
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

	enum class gapi_color_write_mask: uint8_t
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
		rg	= r | g,
		ba	= b | a,
	};
	DEFINE_FLAG_ENUM_CLASS_OPERATORS(gapi_color_write_mask);

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

			bool operator==(const gapi_render_target_blend_desc& other) const
			{
				return color_blend_op == other.color_blend_op &&
					   color_src_blend == other.color_src_blend &&
					   color_dest_blend == other.color_dest_blend &&
					   alpha_blend_op == other.alpha_blend_op &&
					   alpha_src_blend == other.alpha_src_blend &&
					   alpha_dest_blend == other.alpha_dest_blend &&
					   color_write_mask == other.color_write_mask;
			}
		};

		gapi_blend_state_desc() = default;

		gapi_blend_state_desc(const gapi_render_target_blend_desc& in_render_target_blend_desc, bool in_use_alpha_to_coverage = false)
			: m_use_alpha_to_coverage(in_use_alpha_to_coverage)
		{
			m_render_target_blend_descs[0] = in_render_target_blend_desc;
		}

		template<uint32_t num_render_targets>
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

		bool operator==(const gapi_blend_state_desc& other) const
		{
			return m_use_alpha_to_coverage == other.m_use_alpha_to_coverage &&
				   m_use_independent_blend == other.m_use_independent_blend &&
				   m_render_target_blend_descs == other.m_render_target_blend_descs;
		}
	};


	/**
	 *	Rasterizer State
	 */
	enum class gapi_rasterizer_fill_mode: uint8_t
	{
		wireframe,
		solid,
	};

	enum class gapi_rasterizer_cull_mode : uint8_t
	{
		// dont cull anything
		cull_none,
		// cull back face ( clock wise )
		cull_back,
		// cull front face ( counter-clock wise )
		cull_front,
	};

	enum class gapi_rasterizer_depth_clip_mode : uint8_t
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

		bool operator==(const gapi_rasterizer_state_desc& other) const
		{
			return m_fill_mode == other.m_fill_mode &&
				   m_cull_mode == other.m_cull_mode &&
				   m_depth_clip_mode == other.m_depth_clip_mode &&
				   m_depth_bias == other.m_depth_bias &&
				   m_slope_scale_depth_bias == other.m_slope_scale_depth_bias &&
				   m_use_msaa == other.m_use_msaa &&
				   m_use_line_aa == other.m_use_line_aa;
		}
	};


	/**
	 *	Depth Stencil State
	 */
	enum class gapi_cmp_func : uint8_t
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

	enum class gapi_stencil_op : uint8_t
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

			bool operator==(const gapi_stencil_state_desc& other) const
			{
				return m_use_stencil == other.m_use_stencil &&
					   m_stencil_func == other.m_stencil_func &&
					   m_stencil_fail_op == other.m_stencil_fail_op &&
					   m_depth_fail_op == other.m_depth_fail_op &&
					   m_pass_op == other.m_pass_op;
			}
		};
		gapi_stencil_state_desc m_front_stencil_test;
		gapi_stencil_state_desc m_back_stencil_test;
		uint8_t m_stencil_read_mask;
		uint8_t m_stencil_write_mask;

		// Default Constructor
		gapi_depth_stencil_state_desc();

		bool operator==(const gapi_depth_stencil_state_desc& other) const
		{
			return m_use_depth_write == other.m_use_depth_write &&
				   m_depth_func == other.m_depth_func &&
				   m_front_stencil_test == other.m_front_stencil_test &&
				   m_back_stencil_test == other.m_back_stencil_test &&
				   m_stencil_read_mask == other.m_stencil_read_mask &&
				   m_stencil_write_mask == other.m_stencil_write_mask;
		}
	};

	/**
	 *	The argument that needed to create a pipeline state object.
	 */
	struct NENE_API gapi_compute_pipeline_state_desc
	{
		gapi_bound_shader_state_desc m_bound_shader_state;

		gapi_compute_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state);

		bool operator==(const gapi_compute_pipeline_state_desc& other) const
		{
			return m_bound_shader_state == other.m_bound_shader_state;
		}

		size_t hash() const;
	};

	struct NENE_API gapi_graphics_pipeline_state_desc
	{
		// shaders
		gapi_bound_shader_state_desc m_bound_shader_state;
		// RS
		gapi_rasterizer_state_desc m_rasterizer_state;
		gapi_primitive_type m_primitive_type = gapi_primitive_type::triangle;
		// OM
		uint16_t m_num_samples = 1;
		gapi_blend_state_desc m_blend_state;
		gapi_depth_stencil_state_desc m_depth_stencil_state;
		gapi_pixel_format m_depth_stencil_format = gapi_pixel_format::unknown;
		std::vector<gapi_pixel_format> m_render_target_formats;

		// Minimal Constructor
		gapi_graphics_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state);

		bool operator==(const gapi_graphics_pipeline_state_desc& other) const
		{
			return m_bound_shader_state == other.m_bound_shader_state &&
				   m_rasterizer_state == other.m_rasterizer_state &&
				   m_primitive_type == other.m_primitive_type &&
				   m_num_samples == other.m_num_samples &&
				   m_blend_state == other.m_blend_state &&
				   m_depth_stencil_state == other.m_depth_stencil_state &&
				   m_depth_stencil_format == other.m_depth_stencil_format &&
				   m_render_target_formats == other.m_render_target_formats;
		}

		size_t hash() const;
	};
}

#include "gapi_pipeline_state_desc.inl"

namespace nene
{
	inline size_t gapi_compute_pipeline_state_desc::hash() const
	{
		return std::hash<gapi_compute_pipeline_state_desc>{}(*this);
	}

	inline size_t gapi_graphics_pipeline_state_desc::hash() const
	{
		return std::hash<gapi_graphics_pipeline_state_desc>{}(*this);
	}
}