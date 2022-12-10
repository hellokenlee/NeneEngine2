/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_device.h"
#include "d3d12_globals.h"
#include "d3d12_descriptor_heap.h"
#include "d3d12_cmd_list_mgr.h"

#include <windows.h>


d3d12_device::d3d12_device(t::shared_ptr<d3d12_adapter> adapter)
	: d3d12_adapter_child(adapter)
{
	// Create device
	VERIFY(D3D12CreateDevice(get_parent_adapter()->get_dxgi_adapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));

	// Create root signature
	CD3DX12_ROOT_SIGNATURE_DESC desc;
	desc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	WinComPtr<ID3DBlob> signature;
	WinComPtr<ID3DBlob> error;
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	VERIFY(
		m_device->CreateRootSignature(
			0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
		)
	);
}

d3d12_device::d3d12_device(t::shared_ptr<d3d12_adapter> adapter, ID3D12Device* d3d_device)
	: d3d12_adapter_child(adapter)
{
	m_device = d3d_device;
}

d3d12_device::~d3d12_device()
{
	clear();
}

void d3d12_device::init()
{
	// Register device
	get_parent_adapter()->append_device(shared_from_this());

	// Create global heap
	m_global_descriptor_heap = t::shared_ptr<d3d12_descriptor_heap>(
		new d3d12_descriptor_heap(shared_from_this(), g_d3d12_max_global_descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	// Create command list manager
	m_copy_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(this->shared_from_this(), d3d12_cmd_type::copy);
	m_compute_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(shared_from_this(), d3d12_cmd_type::compute);
	m_graphics_cmd_list_mgr = t::make_shared<d3d12_cmd_list_mgr>(shared_from_this(), d3d12_cmd_type::graphics);
}

void d3d12_device::clear()
{
	m_copy_cmd_list_mgr.reset();
	m_compute_cmd_list_mgr.reset();
	m_graphics_cmd_list_mgr.reset();
	m_global_descriptor_heap.reset();
}

t::shared_ptr<d3d12_cmd_list_mgr> d3d12_device::get_cmd_list_mgr(d3d12_cmd_type type)
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