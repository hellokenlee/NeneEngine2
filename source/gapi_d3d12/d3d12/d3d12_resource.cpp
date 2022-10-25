/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_resource.h"

d3d12_resource::d3d12_resource(t::shared_ptr<d3d12_device> device)
	: d3d12_resource(device, device->get_global_descriptor_heap())
{
}

d3d12_resource::d3d12_resource(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap)
	: d3d12_device_child(device)
	, m_located_heap(heap)
{

}