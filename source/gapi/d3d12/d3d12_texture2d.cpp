/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_texture2d.h"

d3d12_texture2d::d3d12_texture2d(shared_ptr<d3d12_device> device)
	: d3d12_texture2d(device, device->get_global_descriptor_heap())
{

}

d3d12_texture2d::d3d12_texture2d(shared_ptr<d3d12_device> device, shared_ptr<d3d12_descriptor_heap> heap)
	: d3d12_resource(device)
{

}

void d3d12_texture2d::init()
{
	CHECK(m_rtv == nullptr);
	m_rtv = shared_ptr<d3d12_render_target_view>(new d3d12_render_target_view(shared_from_this()));
}
