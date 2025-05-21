/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_cmd_allocator.h"
#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_descriptor.h"
#include "gapi_d3d12_resource.h"
#include "d3d12_type_cast.h"


gapi_d3d12_cmd_list::gapi_d3d12_cmd_list(const WinComPtr<ID3D12GraphicsCommandList>& list)
	: m_list(list)
{}

gapi_d3d12_cmd_list::~gapi_d3d12_cmd_list()
{
	// TODO: Delete `m_list`
}

void gapi_d3d12_cmd_list::close()
{
	m_list->Close();
}

void gapi_d3d12_cmd_list::reset(const t::shared_ptr<i::gapi_cmd_allocator>& allocator,
	const t::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	const auto d3d_allocator = gapi_d3d12_cmd_allocator::cast(allocator);
	m_list->Reset(d3d_allocator->get_d3d_allocator(), pipeline_state ? gapi_d3d12_pipeline_state::cast(pipeline_state)->m_pipeline_state.Get() : nullptr);
}

void gapi_d3d12_cmd_list::clear_state(const t::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	const auto d3d_pipeline_state = gapi_d3d12_pipeline_state::cast(pipeline_state);
	m_list->ClearState(d3d_pipeline_state->m_pipeline_state.Get());
}

void gapi_d3d12_cmd_list::clear_depth_stencil_view(const t::shared_ptr<i::gapi_depth_stencil_view>& depth_stencil, const float& depth, const uint8& stencil)
{
	const auto d3d_view = gapi_d3d12_descriptor::cast(depth_stencil);
	m_list->ClearDepthStencilView(d3d_view->get_d3d_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_render_target_view(const t::shared_ptr<i::gapi_render_target_view>& render_target, const linear_color& clear_color)
{
	const auto d3d_view = gapi_d3d12_descriptor::cast(render_target);
	m_list->ClearRenderTargetView(d3d_view->get_d3d_cpu_handle(), clear_color._rgba, 0, nullptr);
}

void gapi_d3d12_cmd_list::clear_unordered_access_view(const t::shared_ptr<i::gapi_unorder_access_view>& unorder_access_view, const t::shared_ptr<i::gapi_resource>& resource, const linear_color& clear_color)
{
	// Floating point format
	const auto& d3d_view = gapi_d3d12_descriptor::cast(unorder_access_view);
	const auto& d3d_resource = gapi_d3d12_resource::cast(resource);
	if (resource->get_resource_desc().m_format == gapi_pixel_format::r8g8b8a8)
	{
		m_list->ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view->get_d3d_cpu_handle(), d3d_resource->get_d3d_resource(), clear_color._rgba, 0, nullptr);
	}
	// Unsigned integer format
	else
	{
		uint32 color[4];
		color[0] = static_cast<uint32>(clear_color.r);
		color[1] = static_cast<uint32>(clear_color.g);
		color[2] = static_cast<uint32>(clear_color.b);
		color[3] = static_cast<uint32>(clear_color.a);
		m_list->ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE(), d3d_view->get_d3d_cpu_handle(), d3d_resource->get_d3d_resource(), color, 0, nullptr);
	}
	// TODO: Other formats...
}

void gapi_d3d12_cmd_list::copy_resource(const t::shared_ptr<i::gapi_resource>& dst, const t::shared_ptr<i::gapi_resource>& src)
{
	const auto& d3d_dst = gapi_d3d12_resource::cast(dst);
	const auto& d3d_src = gapi_d3d12_resource::cast(src);

	m_list->CopyResource(d3d_dst->get_d3d_resource(), d3d_src->get_d3d_resource());
}

void gapi_d3d12_cmd_list::copy_resource_region(const t::shared_ptr<i::gapi_resource>& dst, const uint64& dst_offset, const t::shared_ptr<i::gapi_resource>& src, const uint64& src_offset, const uint64& num_bytes)
{
	CHECK(dst->get_resource_desc().m_type == dst->get_resource_desc().m_type);
	const auto& d3d_dst = gapi_d3d12_resource::cast(dst);
	const auto& d3d_src = gapi_d3d12_resource::cast(src);
	
	if (gapi_resource_desc::is_buffer_desc(dst->get_resource_desc()))
	{
		m_list->CopyBufferRegion(d3d_dst->get_d3d_resource(), dst_offset, d3d_src->get_d3d_resource(), src_offset, num_bytes);
	}
	else if (gapi_resource_desc::is_texture_desc(dst->get_resource_desc()))
	{
		CHECK(false);
		// m_list->CopyTextureRegion()
	}
	else
	{
		CHECK(false);
	}
}

void gapi_d3d12_cmd_list::discard_resource(const t::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_resource = gapi_d3d12_resource::cast(resource);
	m_list->DiscardResource(d3d_resource->get_d3d_resource(), nullptr);
}

void gapi_d3d12_cmd_list::dispatch(const uvector3& thread_group_size)
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

void gapi_d3d12_cmd_list::execute_indirect(const t::shared_ptr<i::gapi_cmd_layout>& layout, const uint32& max_num_cmd, const t::shared_ptr<i::gapi_buffer>& arg_buffer, const uint32& arg_buffer_offset, const t::shared_ptr<i::gapi_buffer>& count_buffer,
	const uint32& count_buffer_offset)
{
	CHECK(false);
	// m_list->ExecuteIndirect()
}

void gapi_d3d12_cmd_list::set_pipeline_state(const t::shared_ptr<i::gapi_pipeline_state>& pipeline_state)
{
	const auto& d3d_pipeline_state = gapi_d3d12_pipeline_state::cast(pipeline_state);
	m_list->SetPipelineState(d3d_pipeline_state->m_pipeline_state.Get());
}

void gapi_d3d12_cmd_list::set_root_constant_buffer_view(const t::shared_ptr<i::gapi_constant_buffer_view>& cbv)
{
}

void gapi_d3d12_cmd_list::set_root_shader_resource_view(const t::shared_ptr<i::gapi_shader_resource_view>& srv)
{
}

void gapi_d3d12_cmd_list::set_root_unordered_access_view(const t::shared_ptr<i::gapi_shader_resource_view>& srv)
{
}

void gapi_d3d12_cmd_list::set_root_descriptor_table()
{
}

void gapi_d3d12_cmd_list::set_descriptor_heaps(const t::dynamic_array<t::shared_ptr<i::gapi_descriptor_heap>>& heaps)
{
}

void gapi_d3d12_cmd_list::set_index_buffer(const t::shared_ptr<i::gapi_index_buffer_view>& index_buffer)
{
	CHECK(false);
	// m_list->IASetIndexBuffer()
}

void gapi_d3d12_cmd_list::set_vertex_buffer(const t::shared_ptr<i::gapi_vertex_buffer_view>& vertex_buffer)
{
	CHECK(false);
	// m_list->IASetVertexBuffers()
}

void gapi_d3d12_cmd_list::set_primitive_topology(const gapi_primitive_type& ptype)
{
	CHECK(false);
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

void gapi_d3d12_cmd_list::set_viewports(const t::dynamic_array<gapi_viewport_desc>& viewports)
{
	m_list->RSSetViewports(static_cast<uint32>(viewports.size()), reinterpret_cast<const D3D12_VIEWPORT*>(viewports.data()));
}

void gapi_d3d12_cmd_list::set_scissor_rects(const t::dynamic_array<rect>& scissors)
{
	m_list->RSSetScissorRects(static_cast<uint32>(scissors.size()), reinterpret_cast<const RECT*>(scissors.data()));
}

void gapi_d3d12_cmd_list::set_blend_factor(const vector4& blend)
{
	m_list->OMSetBlendFactor(&blend.x);
}

void gapi_d3d12_cmd_list::set_render_targets(const t::dynamic_array<t::shared_ptr<i::gapi_render_target_view>>& render_target_views, const t::shared_ptr<i::gapi_depth_stencil_view>& depth_stencil_view)
{
	t::dynamic_array<D3D12_CPU_DESCRIPTOR_HANDLE> d3d_handles;
	for (const auto& view : render_target_views)
	{
		const auto& d3d_view = gapi_d3d12_render_target_view::cast(view);
		d3d_handles.push_back(d3d_view->get_d3d_cpu_handle());
	}
	
	const auto& d3d_depth_stencil_view = gapi_d3d12_depth_stencil_view::cast(depth_stencil_view);
	
	m_list->OMSetRenderTargets(
		static_cast<uint32>(d3d_handles.size()),
		d3d_handles.data(),
		false,
		&(d3d_depth_stencil_view->get_d3d_cpu_handle())
	);
}

void gapi_d3d12_cmd_list::set_stencil_ref(const uint32& stencil_ref)
{
	m_list->OMSetStencilRef(stencil_ref);
}

void gapi_d3d12_cmd_list::transition_resource(const t::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& transition)
{
	CHECK(false);
	// m_list->ResourceBarrier();
}

void gapi_d3d12_cmd_list::begin_query()
{
	CHECK(false);
	// m_list->BeginQuery()
}

void gapi_d3d12_cmd_list::end_query()
{
	CHECK(false);
	// m_list->EndQuery()
}

void gapi_d3d12_cmd_list::resolve_query()
{
	CHECK(false);
	// m_list->ResolveQueryData()
}

void gapi_d3d12_cmd_fence::singal(const uint64& value)
{
	VERIFY(m_fence->Signal(value));
}

gapi_d3d12_cmd_fence::gapi_d3d12_cmd_fence(const WinComPtr<ID3D12Fence>& fence)
	: m_fence(fence)
{
}