/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_cmd_allocator.h"
#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_resource_view.h"
#include "gapi_d3d12_resource_view_allocator.h"
#include "d3d12_type_cast.h"


gapi_d3d12_cmd_list::gapi_d3d12_cmd_list(const WinComPtr<ID3D12GraphicsCommandList>& list)
	: m_d3d_list(list)
{}

gapi_d3d12_cmd_list::~gapi_d3d12_cmd_list() = default;

void gapi_d3d12_cmd_list::close()
{
	m_d3d_list->Close();
}

void gapi_d3d12_cmd_list::reset(const std::shared_ptr<i::gapi_cmd_allocator>& allocator, const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	//
	auto& d3d_allocator = t::gapi_pin<gapi_d3d12_cmd_allocator>(allocator);
	m_d3d_list->Reset(d3d_allocator.get_d3d_allocator(), pipeline_state ? t::gapi_pin<gapi_d3d12_pipeline_state>(pipeline_state).get_d3d_pipeline_state() : nullptr);
	//
	for (auto& d3d_heap : m_d3d_descriptor_heaps)
	{
		d3d_heap = nullptr;
	}
}

void gapi_d3d12_cmd_list::clear_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	m_d3d_list->ClearState(t::gapi_pin<gapi_d3d12_pipeline_state>(pipeline_state).get_d3d_pipeline_state());
}

void gapi_d3d12_cmd_list::clear_depth_stencil_view(const std::shared_ptr<i::gapi_resource_view>& depth_stencil, const float& depth, const uint8& stencil)
{
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(depth_stencil);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::depth_stencil_view);
	m_d3d_list->ClearDepthStencilView(d3d_view.get_d3d_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_render_target_view(const std::shared_ptr<i::gapi_resource_view>& render_target, const color::rgba<float>& clear_color)
{
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(render_target);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::render_target_view);
	m_d3d_list->ClearRenderTargetView(d3d_view.get_d3d_cpu_handle(), clear_color.container().data(), 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_unordered_access_view(const std::shared_ptr<i::gapi_resource_view>& unorder_access_view, const std::shared_ptr<i::gapi_resource>& resource, const color::rgba<float>& clear_color)
{
	// Floating point format
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(unorder_access_view);
	auto& d3d_resource = t::gapi_pin<gapi_d3d12_resource>(resource);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::unordered_access_view);
	if (resource->get_resource_desc().m_format == gapi_pixel_format::r8g8b8a8)
	{
		m_d3d_list->ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view.get_d3d_cpu_handle(), d3d_resource.get_d3d_resource(), clear_color.container().data(), 0, nullptr);
	}
	// Unsigned integer format
	else
	{
		color::rgba<uint32> integer_color;
		integer_color = clear_color;
		m_d3d_list->ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view.get_d3d_cpu_handle(), d3d_resource.get_d3d_resource(), integer_color.container().data(), 0, nullptr);
	}
	// TODO: Other formats...
}

void gapi_d3d12_cmd_list::copy_resource(const std::shared_ptr<i::gapi_resource>& dst, const std::shared_ptr<i::gapi_resource>& src)
{
	const auto& d3d_dst = t::gapi_cast<gapi_d3d12_resource>(dst);
	const auto& d3d_src = t::gapi_cast<gapi_d3d12_resource>(src);

	m_d3d_list->CopyResource(d3d_dst->get_d3d_resource(), d3d_src->get_d3d_resource());
}

void gapi_d3d12_cmd_list::copy_buffer_region(const std::shared_ptr<i::gapi_buffer>& dst, uint32 dst_offset, const std::shared_ptr<i::gapi_buffer>& src, uint32 src_offset, uint64_t num_bytes)
{
	// buffer -> buffer copy
	const auto& d3d_dst = t::gapi_cast<gapi_d3d12_buffer>(dst);
	const auto& d3d_src = t::gapi_cast<gapi_d3d12_buffer>(src);
	//
	m_d3d_list->CopyBufferRegion(d3d_dst->get_d3d_resource(), dst_offset, d3d_src->get_d3d_resource(), src_offset, num_bytes);
}

void gapi_d3d12_cmd_list::copy_texture_region(const std::shared_ptr<i::gapi_texture>& dst, uint32 dst_subindex, const std::shared_ptr<i::gapi_texture>& src, uint32 src_subindex)
{
	// texture -> texture copy
	auto& d3d_dst = t::gapi_pin<gapi_d3d12_texture>(dst);
	auto& d3d_src = t::gapi_pin<gapi_d3d12_texture>(src);
	//
	D3D12_TEXTURE_COPY_LOCATION dst_location {
		.pResource = d3d_dst.get_d3d_resource(),
		.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex = dst_subindex,
	};
	D3D12_TEXTURE_COPY_LOCATION src_location {
		.pResource = d3d_src.get_d3d_resource(),
		.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex = src_subindex,
	};
	// TODO: supports src and dst areas
	m_d3d_list->CopyTextureRegion(&dst_location, 0,0,0, &src_location, nullptr);
}

void gapi_d3d12_cmd_list::copy_buffer_region(const std::shared_ptr<i::gapi_texture>& dst, uint32 dst_subindex, const std::shared_ptr<i::gapi_buffer>& src, const std::shared_ptr<i::gapi_buffer_sublayout>& src_sublayout)
{
	// buffer -> texture copy
	auto& d3d_dst = t::gapi_pin<gapi_d3d12_texture>(dst);
	auto& d3d_src = t::gapi_pin<gapi_d3d12_buffer>(src);

	ID3D12Device* d3d_device = nullptr;
	d3d_dst.get_d3d_resource()->GetDevice(__uuidof(*d3d_device), reinterpret_cast<void**>(&d3d_device));
	//
	D3D12_TEXTURE_COPY_LOCATION dst_location {
		.pResource = d3d_dst.get_d3d_resource(),
		.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
		.SubresourceIndex = dst_subindex,
	};
	D3D12_TEXTURE_COPY_LOCATION src_location {
		.pResource = d3d_src.get_d3d_resource(),
		.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
		.PlacedFootprint = t::gapi_pin<gapi_d3d12_buffer_sublayout>(src_sublayout).m_d3d_layout
	};
	// TODO: supports src and dst areas
	m_d3d_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);
}

void gapi_d3d12_cmd_list::discard_resource(const std::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_resource>(resource);
	m_d3d_list->DiscardResource(d3d_resource->get_d3d_resource(), nullptr);
}

void gapi_d3d12_cmd_list::dispatch(const uint3& thread_group_size)
{
	m_d3d_list->Dispatch(thread_group_size.x, thread_group_size.y, thread_group_size.z);
}

void gapi_d3d12_cmd_list::draw(uint32 num_vertices, uint32 num_instances, uint32 vertex_offset, uint32 instance_offset)
{
	m_d3d_list->DrawInstanced(num_vertices, num_instances, vertex_offset, instance_offset);
}

void gapi_d3d12_cmd_list::draw_indexed(uint32 num_indices, uint32 num_instances, uint32 index_offset, uint32 vertex_offset, uint32 instance_offset)
{
	m_d3d_list->DrawIndexedInstanced(num_indices, num_instances, index_offset, vertex_offset, instance_offset);
}

void gapi_d3d12_cmd_list::execute_indirect(const std::shared_ptr<i::gapi_cmd_layout>& layout, uint32 max_num_cmd, const std::shared_ptr<i::gapi_buffer>& arg_buffer, uint32 arg_buffer_offset, const std::shared_ptr<i::gapi_buffer>& count_buffer,
	uint32 count_buffer_offset)
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::set_pipeline_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	auto& d3d_pipeline_state = t::gapi_pin<gapi_d3d12_pipeline_state>(pipeline_state);

	// TODO: Share root signature across pipeline states
	// refs: https://stackoverflow.com/questions/38535725/what-is-the-point-of-d3d12s-setgraphicsrootsignature
	// 单独设计一个接口来绑定 RootSignature 而不是直接绑定创建 PipelineStateObject的 RootSignature 是为了能够在多个不同的 PipelineState 复用
	// 比如说同一组 Shader 可能会创建多个 PSO 这时候就可以在这些 PSO 中共享这个 Shader 编译出来的 RootSignature 来节省内存以及 CPU 调用的开销
	if (d3d_pipeline_state.is_graphics())
	{
		m_d3d_list->SetGraphicsRootSignature(d3d_pipeline_state.get_d3d_root_signature());	
	}
	else
	{
		NOT_IMPLEMENTED();
	}
	m_d3d_list->SetPipelineState(d3d_pipeline_state.get_d3d_pipeline_state());
}

void gapi_d3d12_cmd_list::bind_root_constant_buffer(uint32_t parameter_index, const std::shared_ptr<i::gapi_buffer>& buffer)
{
	const auto& d3d_buffer = t::gapi_pin<gapi_d3d12_buffer>(buffer);
	m_d3d_list->SetGraphicsRootConstantBufferView(parameter_index, d3d_buffer.get_d3d_resource()->GetGPUVirtualAddress());
}

void gapi_d3d12_cmd_list::bind_root_shader_resource(uint32_t parameter_index, const std::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_resource = t::gapi_pin<gapi_d3d12_resource>(resource);
	m_d3d_list->SetGraphicsRootShaderResourceView(parameter_index, d3d_resource.get_d3d_resource()->GetGPUVirtualAddress());
}

void gapi_d3d12_cmd_list::bind_root_unordered_access(uint32_t parameter_index, const std::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_resource = t::gapi_pin<gapi_d3d12_resource>(resource);
	m_d3d_list->SetGraphicsRootUnorderedAccessView(parameter_index, d3d_resource.get_d3d_resource()->GetGPUVirtualAddress());
}

void gapi_d3d12_cmd_list::bind_ranged_resource_views(uint32_t parameter_index, const std::shared_ptr<i::gapi_resource_view>& resource_view, const std::shared_ptr<i::gapi_resource_view_allocator>& allocator)
{
	// 置脏并更新当前使用的堆
	bool heap_dirty = false;
	const auto& d3d_allocator = t::gapi_pin<gapi_d3d12_online_resource_view_frame_allocator>(allocator);
	if (m_d3d_descriptor_heaps[d3d_allocator.get_d3d_heap_type()] != d3d_allocator.get_d3d_heap().Get())
	{
		heap_dirty = true;
		m_d3d_descriptor_heaps[d3d_allocator.get_d3d_heap_type()] = d3d_allocator.get_d3d_heap().Get();
	}
	if (heap_dirty)
	{
		// You can only bind descriptor heaps of type D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV and D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER.
		// Only one descriptor heap of each type can be set at one time, which means a maximum of 2 heaps (one sampler, one CBV/SRV/UAV) can be set at one time.
		// refs: https://learn.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12graphicscommandlist-setdescriptorheaps
		std::vector<ID3D12DescriptorHeap*> descriptor_heaps;
		for (auto descriptor_heap : m_d3d_descriptor_heaps)
		{
			if (descriptor_heap != nullptr)
			{
				descriptor_heaps.emplace_back(descriptor_heap);
			}
		}
		m_d3d_list->SetDescriptorHeaps(static_cast<UINT>(descriptor_heaps.size()), descriptor_heaps.data());
	}
	
	// 设置绑定所需的
	const auto& online_resource_view = t::gapi_pin<gapi_d3d12_online_resource_view>(resource_view);
	m_d3d_list->SetGraphicsRootDescriptorTable(parameter_index, online_resource_view.get_d3d_gpu_handle());
}

void gapi_d3d12_cmd_list::set_index_buffer(const std::shared_ptr<i::gapi_buffer>& index_buffer)
{
	auto& d3d12_index_buffer = t::gapi_pin<gapi_d3d12_buffer>(index_buffer);
	m_d3d_list->IASetIndexBuffer(&d3d12_index_buffer.m_optional_index_buffer_view);
}

void gapi_d3d12_cmd_list::set_vertex_buffer(const std::shared_ptr<i::gapi_buffer>& vertex_buffer)
{
	auto& d3d12_vertex_buffer = t::gapi_pin<gapi_d3d12_buffer>(vertex_buffer);
	m_d3d_list->IASetVertexBuffers(0, 1, &d3d12_vertex_buffer.m_optional_vertex_buffer_view);
}

void gapi_d3d12_cmd_list::set_primitive_topology(const gapi_primitive_type& ptype)
{
	D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	switch (ptype)
	{
	case gapi_primitive_type::point: topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case gapi_primitive_type::line: topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case gapi_primitive_type::triangle: topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case gapi_primitive_type::patch: topology = D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST; break;
	}
	m_d3d_list->IASetPrimitiveTopology(topology);
}

void gapi_d3d12_cmd_list::set_viewports(const std::vector<gapi_viewport_desc>& viewports)
{
	m_d3d_list->RSSetViewports(static_cast<uint32>(viewports.size()), reinterpret_cast<const D3D12_VIEWPORT*>(viewports.data()));
}

void gapi_d3d12_cmd_list::set_scissor_rects(const std::vector<rect>& scissors)
{
	m_d3d_list->RSSetScissorRects(static_cast<uint32>(scissors.size()), reinterpret_cast<const RECT*>(scissors.data()));
}

void gapi_d3d12_cmd_list::set_blend_factor(const float4& blend)
{
	m_d3d_list->OMSetBlendFactor(&blend.x);
}

void gapi_d3d12_cmd_list::set_render_targets(const std::vector<std::shared_ptr<i::gapi_resource_view>>& render_target_views, const std::shared_ptr<i::gapi_resource_view>& depth_stencil_view)
{
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> d3d_handles;
	for (const auto& view : render_target_views)
	{
		CHECK(view->get_type() == gapi_resource_view_type::render_target_view);
		auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(view);
		d3d_handles.push_back(d3d_view.get_d3d_cpu_handle());
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE* depth_stencil_descriptor_handle = nullptr;
	if (depth_stencil_view != nullptr)
	{
		CHECK(depth_stencil_view->get_type() == gapi_resource_view_type::depth_stencil_view);
		auto& d3d_depth_stencil_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(depth_stencil_view);
		depth_stencil_descriptor_handle = &(d3d_depth_stencil_view.get_d3d_cpu_handle());
	}
	
	m_d3d_list->OMSetRenderTargets(
		static_cast<uint32>(d3d_handles.size()),
		d3d_handles.data(),
		false,
		depth_stencil_descriptor_handle
	);
}

void gapi_d3d12_cmd_list::set_stencil_ref(uint32 stencil_ref)
{
	m_d3d_list->OMSetStencilRef(stencil_ref);
}

void gapi_d3d12_cmd_list::transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& to_state)
{
	// TODO: batch independent barriers
	if (auto barrier = t::gapi_pin<gapi_d3d12_resource>(resource).d3d_transition(to_state); barrier.has_value())
	{
		m_d3d_list->ResourceBarrier(1, std::addressof(barrier.value()));		
	}
}

void gapi_d3d12_cmd_list::begin_query()
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::end_query()
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::resolve_query()
{
	NOT_IMPLEMENTED();
}
