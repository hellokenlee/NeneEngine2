/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_cmd_allocator.h"
#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_resource_view.h"
#include "gapi_d3d12_resource.h"
#include "d3d12_type_cast.h"


gapi_d3d12_cmd_list::gapi_d3d12_cmd_list(const WinComPtr<ID3D12GraphicsCommandList>& list)
	: m_list(list)
{}

gapi_d3d12_cmd_list::~gapi_d3d12_cmd_list() = default;

void gapi_d3d12_cmd_list::close()
{
	m_list->Close();
}

void gapi_d3d12_cmd_list::reset(const std::shared_ptr<i::gapi_cmd_allocator>& allocator, const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	//
	auto& d3d_allocator = t::gapi_pin<gapi_d3d12_cmd_allocator>(allocator);
	m_list->Reset(d3d_allocator.get_d3d_allocator(), pipeline_state ? t::gapi_pin<gapi_d3d12_pipeline_state>(pipeline_state).get_d3d_pipeline_state() : nullptr);
}

void gapi_d3d12_cmd_list::clear_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	m_list->ClearState(t::gapi_pin<gapi_d3d12_pipeline_state>(pipeline_state).get_d3d_pipeline_state());
}

void gapi_d3d12_cmd_list::clear_depth_stencil_view(const std::shared_ptr<i::gapi_resource_view>& depth_stencil, const float& depth, const uint8& stencil)
{
	auto& d3d_view = t::gapi_pin<gapi_d3d12_resource_view>(depth_stencil);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::depth_stencil_view);
	m_list->ClearDepthStencilView(d3d_view.get_d3d_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_render_target_view(const std::shared_ptr<i::gapi_resource_view>& render_target, const color::rgba<float>& clear_color)
{
	auto& d3d_view = t::gapi_pin<gapi_d3d12_resource_view>(render_target);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::render_target_view);
	m_list->ClearRenderTargetView(d3d_view.get_d3d_cpu_handle(), clear_color.container().data(), 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_unordered_access_view(const std::shared_ptr<i::gapi_resource_view>& unorder_access_view, const std::shared_ptr<i::gapi_resource>& resource, const color::rgba<float>& clear_color)
{
	// Floating point format
	auto& d3d_view = t::gapi_pin<gapi_d3d12_resource_view>(unorder_access_view);
	auto& d3d_resource = t::gapi_pin<gapi_d3d12_resource>(resource);
	CHECK(d3d_view.get_type() == gapi_resource_view_type::unordered_access_view);
	if (resource->get_resource_desc().m_format == gapi_pixel_format::r8g8b8a8)
	{
		m_list->ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view.get_d3d_cpu_handle(), d3d_resource.get_d3d_resource(), clear_color.container().data(), 0, nullptr);
	}
	// Unsigned integer format
	else
	{
		color::rgba<uint32> integer_color;
		integer_color = clear_color;
		m_list->ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view.get_d3d_cpu_handle(), d3d_resource.get_d3d_resource(), integer_color.container().data(), 0, nullptr);
	}
	// TODO: Other formats...
}

void gapi_d3d12_cmd_list::copy_resource(const std::shared_ptr<i::gapi_resource>& dst, const std::shared_ptr<i::gapi_resource>& src)
{
	const auto& d3d_dst = t::gapi_cast<gapi_d3d12_resource>(dst);
	const auto& d3d_src = t::gapi_cast<gapi_d3d12_resource>(src);

	m_list->CopyResource(d3d_dst->get_d3d_resource(), d3d_src->get_d3d_resource());
}

void gapi_d3d12_cmd_list::copy_resource_region(const std::shared_ptr<i::gapi_resource>& dst, const uint32& dst_offset, const std::shared_ptr<i::gapi_resource>& src, const uint32& src_offset, const uint32& num_bytes)
{
	CHECK(dst->get_resource_desc().m_type == dst->get_resource_desc().m_type);
	const auto& d3d_dst = t::gapi_cast<gapi_d3d12_resource>(dst);
	const auto& d3d_src = t::gapi_cast<gapi_d3d12_resource>(src);
	
	if (dst->get_resource_desc().is_buffer())
	{
		m_list->CopyBufferRegion(d3d_dst->get_d3d_resource(), dst_offset, d3d_src->get_d3d_resource(), src_offset, num_bytes);
	}
	else if (dst->get_resource_desc().is_texture())
	{
		NOT_IMPLEMENTED();
	}
	else
	{
		CHECK(false);
	}
}

void gapi_d3d12_cmd_list::discard_resource(const std::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_resource>(resource);
	m_list->DiscardResource(d3d_resource->get_d3d_resource(), nullptr);
}

void gapi_d3d12_cmd_list::dispatch(const uint3& thread_group_size)
{
	m_list->Dispatch(thread_group_size.x, thread_group_size.y, thread_group_size.z);
}

void gapi_d3d12_cmd_list::draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset, const uint32& instance_offset)
{
	m_list->DrawInstanced(num_vertices, num_instances, vertex_offset, instance_offset);
}

void gapi_d3d12_cmd_list::draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset, const uint32& vertex_offset, const uint32& instance_offset)
{
	m_list->DrawIndexedInstanced(num_indices, num_instances, index_offset, vertex_offset, instance_offset);
}

void gapi_d3d12_cmd_list::execute_indirect(const std::shared_ptr<i::gapi_cmd_layout>& layout, const uint32& max_num_cmd, const std::shared_ptr<i::gapi_buffer>& arg_buffer, const uint32& arg_buffer_offset, const std::shared_ptr<i::gapi_buffer>& count_buffer,
	const uint32& count_buffer_offset)
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
		m_list->SetGraphicsRootSignature(d3d_pipeline_state.get_d3d_root_signature());	
	}
	else
	{
		NOT_IMPLEMENTED();
	}
	m_list->SetPipelineState(d3d_pipeline_state.get_d3d_pipeline_state());
}

void gapi_d3d12_cmd_list::set_root_constant_buffer_view(const std::shared_ptr<i::gapi_resource_view>& cbv)
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::set_root_shader_resource_view(const std::shared_ptr<i::gapi_resource_view>& srv)
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::set_root_unordered_access_view(const std::shared_ptr<i::gapi_resource_view>& srv)
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::set_root_descriptor_table()
{
	NOT_IMPLEMENTED();
}

void gapi_d3d12_cmd_list::set_descriptor_heaps(const std::vector<std::shared_ptr<i::gapi_resource_view_allocator>>& heaps)
{
}

void gapi_d3d12_cmd_list::set_index_buffer(const std::shared_ptr<i::gapi_buffer>& index_buffer)
{
	auto& d3d12_index_buffer = t::gapi_pin<gapi_d3d12_buffer>(index_buffer);
	m_list->IASetIndexBuffer(&d3d12_index_buffer.m_optional_index_buffer_view);
}

void gapi_d3d12_cmd_list::set_vertex_buffer(const std::shared_ptr<i::gapi_buffer>& vertex_buffer)
{
	auto& d3d12_vertex_buffer = t::gapi_pin<gapi_d3d12_buffer>(vertex_buffer);
	m_list->IASetVertexBuffers(0, 1, &d3d12_vertex_buffer.m_optional_vertex_buffer_view);
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
	m_list->IASetPrimitiveTopology(topology);
}

void gapi_d3d12_cmd_list::set_viewports(const std::vector<gapi_viewport_desc>& viewports)
{
	m_list->RSSetViewports(static_cast<uint32>(viewports.size()), reinterpret_cast<const D3D12_VIEWPORT*>(viewports.data()));
}

void gapi_d3d12_cmd_list::set_scissor_rects(const std::vector<rect>& scissors)
{
	m_list->RSSetScissorRects(static_cast<uint32>(scissors.size()), reinterpret_cast<const RECT*>(scissors.data()));
}

void gapi_d3d12_cmd_list::set_blend_factor(const float4& blend)
{
	m_list->OMSetBlendFactor(&blend.x);
}

void gapi_d3d12_cmd_list::set_render_targets(const std::vector<std::shared_ptr<i::gapi_resource_view>>& render_target_views, const std::shared_ptr<i::gapi_resource_view>& depth_stencil_view)
{
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> d3d_handles;
	for (const auto& view : render_target_views)
	{
		CHECK(view->get_type() == gapi_resource_view_type::render_target_view);
		auto& d3d_view = t::gapi_pin<gapi_d3d12_resource_view>(view);
		d3d_handles.push_back(d3d_view.get_d3d_cpu_handle());
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE* depth_stencil_descriptor_handle = nullptr;
	if (depth_stencil_view != nullptr)
	{
		CHECK(depth_stencil_view->get_type() == gapi_resource_view_type::depth_stencil_view);
		auto& d3d_depth_stencil_view = t::gapi_pin<gapi_d3d12_resource_view>(depth_stencil_view);
		depth_stencil_descriptor_handle = &(d3d_depth_stencil_view.get_d3d_cpu_handle());
	}
	
	m_list->OMSetRenderTargets(
		static_cast<uint32>(d3d_handles.size()),
		d3d_handles.data(),
		false,
		depth_stencil_descriptor_handle
	);
}

void gapi_d3d12_cmd_list::set_stencil_ref(const uint32& stencil_ref)
{
	m_list->OMSetStencilRef(stencil_ref);
}

void gapi_d3d12_cmd_list::transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& to_state)
{
	// TODO: batch independent barriers
	if (auto barrier = t::gapi_pin<gapi_d3d12_resource>(resource).d3d_transition(to_state); barrier.has_value())
	{
		m_list->ResourceBarrier(1, std::addressof(barrier.value()));		
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
