/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_resource_view_type : uint8_t
{
	unknown = 0b000000,
	
	constant_buffer_view	= 0b000001,
	shader_resource_view	= 0b000010,
	unordered_access_view	= 0b000100,
	
	texture_sampler			= 0b001000,

	render_target_view		= 0b010000,
	depth_stencil_view		= 0b100000,
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

enum class gapi_compare_func
{
	
};

struct gapi_sampler_desc
{
	gapi_filter m_filer;
	gapi_sample_test m_sample_test;
	gapi_address_mode m_address_mode_uvw[3];
	float m_mipmap_bias;
	uint32_t m_max_anisotropy;
	gapi_compare_func m_compare_func;
	color::rgba<float> m_border_color;
	float m_lod_min_max[2];
};


namespace i
{
	/**
	*	A `gapi_resource_view` is how we to interpret a resource ( a memory in VRAM ).
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

		//
		gapi_resource_view_type get_type() const { return m_type; }
	
	protected:
		void set_type(gapi_resource_view_type type) { m_type = type; }
		
		gapi_resource_view_type m_type = gapi_resource_view_type::unknown;

		friend class gapi_resource_view_allocator;
	};
}
