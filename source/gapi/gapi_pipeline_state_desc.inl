/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <boost/intrusive/detail/hash_combine.hpp>


namespace nene
{
	template <typename T>
	void hash_combine(std::size_t& seed, const T& v)
	{
		boost::intrusive::detail::hash_combine_size_t(seed, std::hash<T>{}(v));
	}
}

namespace std
{
	template <>
	struct hash<nene::gapi_vertex_element_desc>
	{
		size_t operator()(const nene::gapi_vertex_element_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_semantic_name);
			nene::hash_combine(seed, desc.m_semantic_index);
			nene::hash_combine(seed, (uint8_t)desc.m_element_type);
			nene::hash_combine(seed, desc.m_stream_index);
			nene::hash_combine(seed, desc.m_offset);
			nene::hash_combine(seed, (bool)desc.m_use_instance_index);
			nene::hash_combine(seed, desc.m_stride);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_bound_shader_state_desc>
	{
		size_t operator()(const nene::gapi_bound_shader_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			for (const auto& element : desc.get_vertices_declaration())
			{
				nene::hash_combine(seed, element);
			}
			for (uint32_t i = 0; i < nene::NUM_GAPI_SHADER_STAGE; ++i)
			{
				nene::hash_combine(seed, desc.get_shader((nene::gapi_shader_stage)i));
			}
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_blend_state_desc::gapi_render_target_blend_desc>
	{
		size_t operator()(const nene::gapi_blend_state_desc::gapi_render_target_blend_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, (uint8_t)desc.color_blend_op);
			nene::hash_combine(seed, (uint8_t)desc.color_src_blend);
			nene::hash_combine(seed, (uint8_t)desc.color_dest_blend);
			nene::hash_combine(seed, (uint8_t)desc.alpha_blend_op);
			nene::hash_combine(seed, (uint8_t)desc.alpha_src_blend);
			nene::hash_combine(seed, (uint8_t)desc.alpha_dest_blend);
			nene::hash_combine(seed, (uint8_t)desc.color_write_mask);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_blend_state_desc>
	{
		size_t operator()(const nene::gapi_blend_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_use_alpha_to_coverage);
			nene::hash_combine(seed, desc.m_use_independent_blend);
			for (const auto& rt_blend : desc.m_render_target_blend_descs)
			{
				nene::hash_combine(seed, rt_blend);
			}
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_rasterizer_state_desc>
	{
		size_t operator()(const nene::gapi_rasterizer_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, (uint8_t)desc.m_fill_mode);
			nene::hash_combine(seed, (uint8_t)desc.m_cull_mode);
			nene::hash_combine(seed, (uint8_t)desc.m_depth_clip_mode);
			nene::hash_combine(seed, desc.m_depth_bias);
			nene::hash_combine(seed, desc.m_slope_scale_depth_bias);
			nene::hash_combine(seed, desc.m_use_msaa);
			nene::hash_combine(seed, desc.m_use_line_aa);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_depth_stencil_state_desc::gapi_stencil_state_desc>
	{
		size_t operator()(const nene::gapi_depth_stencil_state_desc::gapi_stencil_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_use_stencil);
			nene::hash_combine(seed, (uint8_t)desc.m_stencil_func);
			nene::hash_combine(seed, (uint8_t)desc.m_stencil_fail_op);
			nene::hash_combine(seed, (uint8_t)desc.m_depth_fail_op);
			nene::hash_combine(seed, (uint8_t)desc.m_pass_op);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_depth_stencil_state_desc>
	{
		size_t operator()(const nene::gapi_depth_stencil_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_use_depth_write);
			nene::hash_combine(seed, (uint8_t)desc.m_depth_func);
			nene::hash_combine(seed, desc.m_front_stencil_test);
			nene::hash_combine(seed, desc.m_back_stencil_test);
			nene::hash_combine(seed, desc.m_stencil_read_mask);
			nene::hash_combine(seed, desc.m_stencil_write_mask);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_compute_pipeline_state_desc>
	{
		size_t operator()(const nene::gapi_compute_pipeline_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_bound_shader_state);
			return seed;
		}
	};

	template <>
	struct hash<nene::gapi_graphics_pipeline_state_desc>
	{
		size_t operator()(const nene::gapi_graphics_pipeline_state_desc& desc) const noexcept
		{
			size_t seed = 0;
			nene::hash_combine(seed, desc.m_bound_shader_state);
			nene::hash_combine(seed, desc.m_rasterizer_state);
			nene::hash_combine(seed, (uint8_t)desc.m_primitive_type);
			nene::hash_combine(seed, desc.m_num_samples);
			nene::hash_combine(seed, desc.m_blend_state);
			nene::hash_combine(seed, desc.m_depth_stencil_state);
			nene::hash_combine(seed, (uint8_t)desc.m_depth_stencil_format);
			for (const auto& format : desc.m_render_target_formats)
			{
				nene::hash_combine(seed, (uint8_t)format);
			}
			return seed;
		}
	};
}