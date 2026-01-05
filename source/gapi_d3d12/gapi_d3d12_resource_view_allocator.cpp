/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view_allocator.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_resource_view.h"


gapi_d3d12_offline_resource_view_page_allocator::gapi_d3d12_offline_resource_view_page_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, size_t page_size)
	: gapi_resource_view_allocator()
	, m_d3d_device(d3d_device)
	, m_d3d_page_desc{.Type = d3d_cast(view_type), .NumDescriptors = static_cast<UINT>(page_size), .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE, .NodeMask = 0}
	, m_page_size(static_cast<uint32>(page_size))
	, m_resource_view_size(m_d3d_device->GetDescriptorHandleIncrementSize(m_d3d_page_desc.Type))
{}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_offline_resource_view_page_allocator::allocate_resource_view(gapi_resource_view_type vtype)
{
	//
	CHECK(m_d3d_page_desc.Type == d3d_cast(vtype));
	//
	if (m_free_resource_view_indices.empty())
	{
		// allocate new page
		WinComPtr<ID3D12DescriptorHeap> heap;
		VERIFY(m_d3d_device->CreateDescriptorHeap(&m_d3d_page_desc, IID_PPV_ARGS(&heap)));
		//
		uint32 page_index = static_cast<uint32>(m_pages.size());
		m_pages.emplace_back(heap, heap->GetCPUDescriptorHandleForHeapStart());
		//
		for (uint32 index_in_heap = 0; index_in_heap < m_page_size; ++index_in_heap)
		{
			m_free_resource_view_indices.emplace(page_index, static_cast<int32>(index_in_heap));
		}
	}
	// request a free view
	auto index = m_free_resource_view_indices.front();
	m_free_resource_view_indices.pop();
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_pages[index.m_heap_page_index].m_cpu_base);
	cpu_handle.Offset(index.m_index_in_heap, m_resource_view_size);
	//
	auto view = std::shared_ptr<gapi_d3d12_offline_resource_view>{ new gapi_d3d12_offline_resource_view(index, cpu_handle), [this](gapi_d3d12_offline_resource_view* p){ free_resource_view(p); } };
	initialize_resource_view(*view, vtype);
	return view;
}

void gapi_d3d12_offline_resource_view_page_allocator::free_resource_view(i::gapi_resource_view* view)
{
	// FIXME: pages never free here
	auto d3d_view = dynamic_cast<gapi_d3d12_offline_resource_view*>(view);
	m_free_resource_view_indices.push(d3d_view->get_index_in_heap());
}

gapi_d3d12_online_resource_view_frame_allocator::gapi_d3d12_online_resource_view_frame_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, size_t max_size)
	: gapi_resource_view_allocator()
	, m_max_size(static_cast<uint32_t>(max_size))
	, m_d3d_heap_type(d3d_cast(view_type))
{
	//
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
		.Type = m_d3d_heap_type,
		.NumDescriptors = m_max_size,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		.NodeMask = 0
	};
	//
	m_resource_view_size = d3d_device->GetDescriptorHandleIncrementSize(heap_desc.Type);
	//
	VERIFY(d3d_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_d3d_heap)));
}

void gapi_d3d12_online_resource_view_frame_allocator::reset()
{
	m_next_index = 0;
}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_online_resource_view_frame_allocator::allocate_resource_view(gapi_resource_view_type vtype)
{
	CHECK(static_cast<uint32_t>(m_next_index) < m_max_size);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_d3d_heap->GetCPUDescriptorHandleForHeapStart());
	cpu_handle.Offset(m_next_index, m_resource_view_size);
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(m_d3d_heap->GetGPUDescriptorHandleForHeapStart());
	cpu_handle.Offset(m_next_index, m_resource_view_size);
	
	++m_next_index;
	auto view = std::make_shared<gapi_d3d12_online_resource_view>(cpu_handle, gpu_handle);
	initialize_resource_view(*view, vtype);
	return view;
}
