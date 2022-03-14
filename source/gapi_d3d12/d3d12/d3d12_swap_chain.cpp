/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_swap_chain.h"
#include "d3d12_utils.h"
#include "d3d12_device.h"
#include "d3d12_cmd_list.h"
#include "d3d12_cmd_list_mgr.h"


d3d12_swap_chain::d3d12_swap_chain(shared_ptr<d3d12_adapter> adapter, void* hwnd, uint32 back_buffer_num, uint32 multi_sample_num)
	: d3d12_adapter_child(adapter)
	, m_hwnd(static_cast<HWND>(hwnd))
	, m_back_buffer_num(back_buffer_num)
	, m_multi_sample_num(multi_sample_num)
{
	//
	auto device = adapter->get_device(0);

	// Create swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.BufferCount = m_back_buffer_num;
		desc.Width = 800;
		desc.Height = 600;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.SampleDesc.Count = m_multi_sample_num;

		auto factory = adapter->get_dxgi_factory6();
		
		ID3D12CommandQueue* command_queue = device->get_graphics_cmd_list_mgr()->get_d3d_command_queue();

		WinComPtr<IDXGISwapChain1> swap_chain;
		VERIFY(factory->CreateSwapChainForHwnd(
			command_queue, m_hwnd, &desc, nullptr, nullptr, &swap_chain
		));

		VERIFY(swap_chain.As(&m_swap_chain));
		
		factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);
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
}

void d3d12_swap_chain::present()
{
	m_swap_chain->Present(1, 0);
}

uint32 d3d12_swap_chain::get_current_back_buffer_index()
{
	return m_swap_chain->GetCurrentBackBufferIndex();
}