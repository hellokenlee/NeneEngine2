/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_resource_view_type : uint8
{
	constant_buffer_view	= 0b000001,
	shader_resource_view	= 0b000010,
	unordered_access_view	= 0b000100,
	
	texture_sampler			= 0b001000,

	render_target_view		= 0b010000,
	depth_stencil_view		= 0b100000,
};

enum class gapi_filter : uint8
{
	point,
	linear,
	anisotropic,
};

enum class gapi_sample_test : uint8
{
	none,
	minimum,
	maximum,
	comparison,
};

enum class gapi_address_mode : uint8
{
	wrap,
	mirror,
	clamp,
	border,
};

enum class gapi_compare_func
{
	
};

struct gapi_sampler_desc
{
	gapi_filter m_filer;
	gapi_sample_test m_sample_test;
	gapi_address_mode m_address_mode_uvw[3];
	float m_mipmap_bias;
	uint32 m_max_anisotropy;
	gapi_compare_func m_compare_func;
	linear_color m_border_color;
	float m_lod_min_max[2];
};

namespace i
{
	/**
	*	A `gapi_resource_view` is for how to treat ( see ) a resource ( a memory in VRAM ) aka resource view.
	*
	*	Equivalents:
	*		- DX: Accessed by `D3D12_CPU_DESCRIPTOR_HANDLE`, logical object stores in `ID3D12DescriptorHeap`.
	*		- VK: Logical object stores in `VkDescriptorPool`. 
	*		- MT: ``
	*/
	class NENE_API gapi_resource_view : noncopyable
	{
	public:
		gapi_resource_view() = default;
		~gapi_resource_view() override = default;

		// Check if this resource view is submitted to GPU
		virtual bool is_created() = 0;
	};

	class NENE_API gapi_shader_resource_view : virtual public gapi_resource_view
	{
	public:
		gapi_shader_resource_view() = default;
		~gapi_shader_resource_view() override = default;
	};

	class NENE_API gapi_unorder_access_view : virtual public gapi_resource_view
	{
	public:
		gapi_unorder_access_view() = default;
		~gapi_unorder_access_view() override = default;
	};

	class NENE_API gapi_constant_buffer_view : virtual public gapi_resource_view
	{
	public:
		gapi_constant_buffer_view() = default;
		~gapi_constant_buffer_view() override = default;
	};

	class NENE_API gapi_render_target_view : virtual public gapi_resource_view
	{
	public:
		gapi_render_target_view() = default;
		~gapi_render_target_view() override = default;
	};

	class NENE_API gapi_depth_stencil_view : virtual public gapi_resource_view
	{
	public:
		gapi_depth_stencil_view() = default;
		~gapi_depth_stencil_view() override = default;
	};

	class NENE_API gapi_sampler : public gapi_resource_view
	{
	public:
		gapi_sampler() = default;
		~gapi_sampler() override = default;
	};
}

