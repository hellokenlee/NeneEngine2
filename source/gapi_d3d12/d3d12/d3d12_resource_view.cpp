/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_resource_view.h"


d3d12_resource_view::d3d12_resource_view(t::shared_ptr<d3d12_resource> resource, t::shared_ptr<d3d12_descriptor_heap> heap)
	: m_resource(resource)
	, m_located_heap(heap)
	, m_descriptor(nullptr)
{
}

d3d12_render_target_view::d3d12_render_target_view(t::shared_ptr<d3d12_texture> resource, const d3d12_resource_creation_args& args)
	: d3d12_render_target_view(resource, resource->get_parent_device()->get_rtv_descriptor_heap(), args)
{
}

d3d12_render_target_view::d3d12_render_target_view(t::shared_ptr<d3d12_texture> resource, t::shared_ptr<d3d12_descriptor_heap> heap, const d3d12_resource_creation_args& args)
	: d3d12_resource_view(resource, heap)
{
	CHECK(resource != nullptr);
	//
	m_descriptor = heap->allocate_descriptor();
	//
	const auto d3d_device = resource->get_parent_device()->get_d3d_device();
	d3d_device->CreateRenderTargetView(resource->get_d3d_resource(), nullptr, m_descriptor->m_handle);
}

d3d12_shader_resource_view::d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource, const d3d12_resource_creation_args& args)
	: d3d12_shader_resource_view(resource, resource->get_parent_device()->get_global_descriptor_heap(), args)
{
}

d3d12_shader_resource_view::d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource, t::shared_ptr<d3d12_descriptor_heap> heap, const d3d12_resource_creation_args& args)
	: d3d12_resource_view(resource, heap)
{
	CHECK(resource != nullptr);
	//
	m_descriptor = heap->allocate_descriptor();
	//
	const auto d3d_device = resource->get_parent_device()->get_d3d_device();
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = args.Format;
	desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.Texture2D.MipLevels = 1;
	//
	d3d_device->CreateShaderResourceView(resource->get_d3d_resource(), &desc, m_descriptor->m_handle);	
}

