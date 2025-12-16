/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view_page_allocator.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_resource_view.h"

static D3D12_DESCRIPTOR_HEAP_FLAGS d3d_cast(gapi_resource_view_allocator_type allocator_type)
{
	return allocator_type == gapi_resource_view_allocator_type::offline ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
}

gapi_d3d12_offline_resource_view_page_allocator::gapi_d3d12_offline_resource_view_page_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, gapi_resource_view_allocator_type allocator_type, size_t page_size)
	: gapi_resource_view_allocator()
	, m_page_size(static_cast<uint32>(page_size))
	, m_d3d_device(d3d_device)
	, m_page_desc{.Type = d3d_cast(view_type), .NumDescriptors = static_cast<UINT>(page_size), .Flags = d3d_cast(allocator_type), .NodeMask = 0}
	, m_resource_view_size(m_d3d_device->GetDescriptorHandleIncrementSize(m_page_desc.Type))
{}

void gapi_d3d12_offline_resource_view_page_allocator::emplace_page(const WinComPtr<ID3D12DescriptorHeap>& heap)
{
	auto page = std::make_shared<d3d12_offline_page_data>(heap, heap->GetCPUDescriptorHandleForHeapStart());
	m_pages.emplace_back(std::move(page));
}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_offline_resource_view_page_allocator::new_resource_view(const d3d12_resource_view_index& index)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_pages[index.m_page_index]->m_cpu_base);
	cpu_handle.Offset(index.m_index_in_heap, m_resource_view_size);
	return { new gapi_d3d12_offline_resource_view(index, cpu_handle), [this](gapi_d3d12_offline_resource_view* p){ free_resource_view(p); } };
}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_offline_resource_view_page_allocator::allocate_resource_view()
{
	if (m_free_resource_view_indices.empty())
	{
		// allocate new page
		WinComPtr<ID3D12DescriptorHeap> heap;
		VERIFY(m_d3d_device->CreateDescriptorHeap(&m_page_desc, IID_PPV_ARGS(&heap)));
		//
		uint32 page_index = static_cast<uint32>(m_pages.size());
		emplace_page(heap);
		//
		for (int32 index_in_heap = 0; index_in_heap < static_cast<int32>(m_page_size); ++index_in_heap)
		{
			m_free_resource_view_indices.emplace(page_index, index_in_heap);
		}
	}
	auto index = m_free_resource_view_indices.front();
	return new_resource_view(index);
}

void gapi_d3d12_offline_resource_view_page_allocator::free_resource_view(i::gapi_resource_view* view)
{
	auto d3d_view = dynamic_cast<gapi_d3d12_offline_resource_view*>(view);
	m_free_resource_view_indices.push(d3d_view->get_index_in_heap());
	// TODO: Page never free here.
}

void gapi_d3d12_online_resource_view_page_allocator::emplace_page(const WinComPtr<ID3D12DescriptorHeap>& heap)
{
	auto page = std::make_shared<d3d12_online_page_data>();
	page->m_heap = heap;
	page->m_cpu_base = heap->GetCPUDescriptorHandleForHeapStart();
	page->m_gpu_base = heap->GetGPUDescriptorHandleForHeapStart();
	m_pages.emplace_back(std::move(page));
}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_online_resource_view_page_allocator::new_resource_view(const d3d12_resource_view_index& index)
{
	const d3d12_online_page_data& page = *std::static_pointer_cast<d3d12_online_page_data>(m_pages[index.m_page_index]);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(page.m_cpu_base);
	cpu_handle.Offset(index.m_index_in_heap, m_resource_view_size);
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(page.m_gpu_base);
	return { new gapi_d3d12_online_resource_view(index, cpu_handle, gpu_handle), [this](gapi_d3d12_offline_resource_view* p){ free_resource_view(p); } };
}
