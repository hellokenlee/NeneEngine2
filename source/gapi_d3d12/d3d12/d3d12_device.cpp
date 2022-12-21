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

	//
	CD3DX12_DESCRIPTOR_RANGE ranges[1];
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

	CD3DX12_ROOT_PARAMETER root_parameters[1];
	root_parameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	
	// Define static samplers
	D3D12_STATIC_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 0;
	sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler_desc.MinLOD = 0.0f;
	sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
	sampler_desc.ShaderRegister = 0;
	sampler_desc.RegisterSpace = 0;
	sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	// Create root signature
	CD3DX12_ROOT_SIGNATURE_DESC root_sig_desc;
	root_sig_desc.Init(1, root_parameters, 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	WinComPtr<ID3DBlob> signature;
	WinComPtr<ID3DBlob> error;
	D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	VERIFY(
		m_device->CreateRootSignature(
			0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
		)
	);
}

d3d12_device::d3d12_device(t::shared_ptr<d3d12_adapter> adapter, ID3D12Device* d3d_device)
	: d3d12_adapter_child(adapter)
{
	// Record external device
	m_device = d3d_device;
	
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
	m_global_descriptor_heap = t::shared_ptr<d3d12_descriptor_heap>(
		new d3d12_descriptor_heap(shared_from_this(), g_d3d12_max_global_descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	
	// Create command list manager
	LOG(d3d12, info, TXT("Creating command list managers."));
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