/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_viewport.h"
#include "d3d12_utils.h"
#include "d3d12_device.h"
#include "d3d12_cmd_list.h"
#include "d3d12_cmd_list_mgr.h"
#include "d3d12_cmd_allocator.h"


d3d12_viewport::d3d12_viewport(shared_ptr<d3d12_adapter> adapter, void* hwnd, uint32 back_buffer_num, uint32 multi_sample_num)
	: d3d12_adapter_child(adapter)
	, m_hwnd(static_cast<HWND>(hwnd))
	, m_last_fence_value(0)
	, m_back_buffer_num(back_buffer_num)
	, m_multi_sample_num(multi_sample_num)
{
	//
	auto device = adapter->get_device(0);

	// Create swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.BufferCount = 2;
		desc.Width = 800;
		desc.Height = 600;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.SampleDesc.Count = 1;

		auto factory = adapter->get_dxgi_factory6();
		
		ID3D12CommandQueue* command_queue = device->get_graphics_cmd_list_mgr()->get_d3d_command_queue();

		WinComPtr<IDXGISwapChain1> swap_chain;
		VERIFY(factory->CreateSwapChainForHwnd(
			command_queue, m_hwnd, &desc, nullptr, nullptr, &swap_chain
		));

		VERIFY(swap_chain.As(&m_swap_chain));
		
		factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);

		m_back_buffer_index = m_swap_chain->GetCurrentBackBufferIndex();
	}

	// Create back buffers resources
	m_back_buffers.clear();
	for (uint32 i = 0; i < m_back_buffer_num; ++i)
	{
		shared_ptr<d3d12_texture2d> back_buffer(new d3d12_texture2d(device));
		WinComPtr<ID3D12Resource> render_target;
		VERIFY(m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&render_target)));
		back_buffer->set_d3d_resource(render_target);
		m_back_buffers.push_back(back_buffer);
	}

	//
	m_fence = shared_ptr<d3d12_fence>(new d3d12_fence(device));

	//
	m_cmd_list = nullptr;
	m_cmd_allocator = nullptr;
}

void d3d12_viewport::begin_frame()
{
	
}

void d3d12_viewport::end_frame()
{
	auto device = get_parent_adapter()->get_device(0);

	m_last_fence_value = m_fence->signal();

	m_fence->wait(m_last_fence_value);

	m_back_buffer_index = m_swap_chain->GetCurrentBackBufferIndex();

	device->get_cmd_list_mgr(d3d12_cmd_type::graphics)->release_cmd_allocator(m_cmd_allocator);
}

void d3d12_viewport::present()
{
	auto device = get_parent_adapter()->get_device(0);

	//
	m_cmd_allocator = device->get_cmd_list_mgr(d3d12_cmd_type::graphics)->obtain_cmd_allocator();
	m_cmd_list = device->get_graphics_cmd_list_mgr()->create_cmd_list(m_cmd_allocator);
	
	//
	m_cmd_list->reset(m_cmd_allocator);

	//
	auto transition0 = CD3DX12_RESOURCE_BARRIER::Transition(m_back_buffers[m_back_buffer_index]->get_d3d_resource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_cmd_list->get_d3d_graphics_cmd_list()->ResourceBarrier(1, &transition0);

	//
	const float clear_color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_cmd_list->get_d3d_graphics_cmd_list()->ClearRenderTargetView(
		*(m_back_buffers[m_back_buffer_index]->get_render_target_view()->get_d3d_descriptor_handle()), clear_color, 0, nullptr
	);

	//
	auto transition1 = CD3DX12_RESOURCE_BARRIER::Transition(m_back_buffers[m_back_buffer_index]->get_d3d_resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_cmd_list->get_d3d_graphics_cmd_list()->ResourceBarrier(1, &transition1);

	//
	m_cmd_list->close();
	//
	device->get_cmd_list_mgr(d3d12_cmd_type::graphics)->execute_cmd_list(m_cmd_list);

	//
	m_swap_chain->Present(1, 0);
}

