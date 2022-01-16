/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_resource_view.h"

d3d12_render_target_view::d3d12_render_target_view(shared_ptr<d3d12_resource> resource)
{
	auto device = resource->get_parent_device();
	auto desc_heap = resource->get_located_heap();
	m_descriptor = desc_heap->allocate_descriptor();

	device->get_d3d_device()->CreateRenderTargetView(resource->get_d3d_resource(), nullptr, m_descriptor->m_handle);
}