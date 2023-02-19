/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_device.h"
#include "d3d12_globals.h"
#include "d3d12_descriptor_heap.h"
#include "d3d12_cmd_list_mgr.h"
#include "d3d12_root_signature_mgr.h"

#include <windows.h>


d3d12_device::d3d12_device(t::shared_ptr<d3d12_adapter> adapter)
	: d3d12_adapter_child(adapter)
{
	// Create device
	VERIFY(D3D12CreateDevice(get_parent_adapter()->get_dxgi_adapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
}

d3d12_device::d3d12_device(t::shared_ptr<d3d12_adapter> adapter, ID3D12Device* d3d_device)
	: d3d12_adapter_child(adapter)
{
	// Record external device
	m_device = d3d_device;
}

d3d12_device::~d3d12_device()
{
	clear();
}

void d3d12_device::init()
{
	LOG(d3d12, info, TXT("Initializing d3d12 device."));
	
	// Register device
	get_parent_adapter()->append_device(shared_from_this());

	// Create global heap
	m_global_descriptor_heap = t::make_shared<d3d12_descriptor_heap>(
		shared_from_this(), g_d3d12_max_global_descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	);
	m_rtv_descriptor_heap = t::make_shared<d3d12_descriptor_heap>(
		shared_from_this(), g_d3d12_max_global_descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);
	m_dsv_descriptor_heap = t::make_shared<d3d12_descriptor_heap>(
		shared_from_this(), g_d3d12_max_global_descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE_DSV
	);
	
	// Create command list manager
	LOG(d3d12, info, TXT("Creating command list managers."));
	m_copy_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(shared_from_this(), d3d12_cmd_type::copy);
	m_compute_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(shared_from_this(), d3d12_cmd_type::compute);
	m_graphics_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(shared_from_this(), d3d12_cmd_type::graphics);

	//
	LOG(d3d12, info, TXT("Creating root signature list managers."));
	m_root_signature_mgr = t::make_shared<d3d12_root_signature_mgr>(shared_from_this());
}

void d3d12_device::clear()
{
	m_copy_cmd_list_mgr.reset();
	m_compute_cmd_list_mgr.reset();
	m_graphics_cmd_list_mgr.reset();
	m_global_descriptor_heap.reset();
	m_root_signature_mgr.reset();
}

t::shared_ptr<d3d12_cmd_list_mgr> d3d12_device::get_cmd_list_mgr(d3d12_cmd_type type) const
{
	switch (type)
	{
	case d3d12_cmd_type::graphics:
		return get_graphics_cmd_list_mgr();
	case d3d12_cmd_type::compute:
		return get_compute_cmd_list_mgr();
	case d3d12_cmd_type::copy:
		return get_copy_cmd_list_mgr();
	}
	return nullptr;
}