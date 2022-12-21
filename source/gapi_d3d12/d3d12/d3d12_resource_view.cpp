/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_resource_view.h"

d3d12_render_target_view::d3d12_render_target_view(t::shared_ptr<d3d12_resource> resource)
{
	CHECK(resource != nullptr);
	//
	const auto desc_heap = resource->get_located_heap();
	m_descriptor = desc_heap->allocate_descriptor();
	//
	const auto d3d_device = resource->get_parent_device()->get_d3d_device();
	d3d_device->CreateRenderTargetView(resource->get_d3d_resource(), nullptr, m_descriptor->m_handle);
}

D3D12_CPU_DESCRIPTOR_HANDLE* d3d12_render_target_view::get_d3d_descriptor_handle() const
{
	return &(m_descriptor->m_handle);
}

d3d12_shader_resource_view::d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource)
{
	
}

d3d12_unordered_access_view::d3d12_unordered_access_view(t::shared_ptr<d3d12_resource> resource)
{
	
}
