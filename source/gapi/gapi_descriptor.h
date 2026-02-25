/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace nene
{
	enum class gapi_descriptor_type : uint8_t
	{
		constant_buffer_view	= 0b00000001,
		shader_resource_view	= 0b00000010,
		unordered_access_view	= 0b00000100,
		
		texture_sampler			= 0b00001000,

		render_target_view		= 0b00010000,
		depth_stencil_view		= 0b00100000,

		cbv_srv_uav = constant_buffer_view | shader_resource_view | unordered_access_view,
	};

	enum class gapi_filter : uint8_t
	{
		point,
		linear,
		anisotropic,
	};

	enum class gapi_sample_test : uint8_t
	{
		none,
		minimum,
		maximum,
		comparison,
	};

	enum class gapi_address_mode : uint8_t
	{
		wrap,
		mirror,
		clamp,
		border,
	};

	enum class gapi_compare_func : uint8_t
	{
		never,
		less,
		equal,
		greater,
		always,
		
		not_equal,
		less_equal,
		greater_equal,
	};

	struct gapi_sampler_desc
	{
		gapi_filter m_filer;
		gapi_sample_test m_sample_test;
		gapi_address_mode m_address_mode_uvw[3];
		float m_mipmap_bias;
		uint32_t m_max_anisotropy;
		gapi_compare_func m_compare_func;
		float4 m_border_color;
		float m_lod_min_max[2];
	};
	
	/**
	*	A descriptor is for how to treat ( view ) a resource aka. resource view.
	*
	*	Equivalents:
	*		- DX: Logical object stores in `ID3D12DescriptorHeap`. ( Accessed by `D3D12_CPU_DESCRIPTOR_HANDLE` )
	*		- VK: Logical object stores in `VkDescriptorPool`. 
	*		- MT: ``
	*/
	class NENE_API gapi_descriptor : noncopyable
	{
	public:
		gapi_descriptor() = default;
		~gapi_descriptor() override = default;

		// Check if this resouce view is submitted to GPU
		virtual bool is_created() = 0;
	};

	class NENE_API gapi_shader_resource_view : virtual public gapi_descriptor
	{
	public:
		gapi_shader_resource_view() = default;
		~gapi_shader_resource_view() override = default;
	};

	class NENE_API gapi_unorder_access_view : virtual public gapi_descriptor
	{
	public:
		gapi_unorder_access_view() = default;
		~gapi_unorder_access_view() override = default;
	};

	class NENE_API gapi_constant_buffer_view : virtual public gapi_descriptor
	{
	public:
		gapi_constant_buffer_view() = default;
		~gapi_constant_buffer_view() override = default;
	};

	class NENE_API gapi_render_target_view : virtual public gapi_descriptor
	{
	public:
		gapi_render_target_view() = default;
		~gapi_render_target_view() override = default;
	};

	class NENE_API gapi_depth_stencil_view : virtual public gapi_descriptor
	{
	public:
		gapi_depth_stencil_view() = default;
		~gapi_depth_stencil_view() override = default;
	};

	class NENE_API gapi_sampler : public gapi_descriptor
	{
	public:
		gapi_sampler() = default;
		~gapi_sampler() override = default;
	};

	class NENE_API gapi_index_buffer_view
	{
		
	};

	class NENE_API gapi_vertex_buffer_view
	{
		
	};
}

