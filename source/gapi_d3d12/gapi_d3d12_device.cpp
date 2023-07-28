/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_device.h"

#include "gapi_d3d12_cmd_allocator.h"
#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_pipeline_layout.h"
#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_resource_heap.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_descriptor.h"
#include "gapi_d3d12_descriptor_heap.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_shader.h"


gapi_d3d12_device::gapi_d3d12_device(const WinComPtr<ID3D12Device>& device)
	: m_device(device)
{
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::grahpics)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::grahpics);
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::compute)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::compute);
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::copy)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::copy);
}

t::shared_ptr<i::gapi_cmd_fence> gapi_d3d12_device::create_cmd_fence(const uint64& initial_value)
{
	// TODO: LDA setup support
	WinComPtr<ID3D12Fence> fence;
	VERIFY(m_device->CreateFence(initial_value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	return t::make_shared<gapi_d3d12_cmd_fence>(fence);
}

t::shared_ptr<i::gapi_cmd_queue> gapi_d3d12_device::create_cmd_queue(gapi_cmd_type type)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = d3d_cast(type);

	WinComPtr<ID3D12CommandQueue> cmd_queue;
	VERIFY(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&cmd_queue)));
	
	return t::make_shared<gapi_d3d12_cmd_queue>(cmd_queue);
}

t::shared_ptr<i::gapi_cmd_allocator> gapi_d3d12_device::create_cmd_allocator(gapi_cmd_type type)
{
	WinComPtr<ID3D12CommandAllocator> allocator;
	VERIFY(m_device->CreateCommandAllocator(d3d_cast(type), IID_PPV_ARGS(&allocator)));

	return t::make_shared<gapi_d3d12_cmd_allocator>(allocator);
}

t::shared_ptr<i::gapi_cmd_list> gapi_d3d12_device::create_cmd_list(gapi_cmd_type type, t::shared_ptr<i::gapi_cmd_allocator>& allocator)
{
	const auto d3d12_allocator = gapi_d3d12_cmd_allocator::cast(allocator);
	
	WinComPtr<ID3D12GraphicsCommandList> cmd_list;
	VERIFY(m_device->CreateCommandList(0, d3d_cast(type), d3d12_allocator->m_allocator.Get(), nullptr, IID_PPV_ARGS(&cmd_list)));

	return t::make_shared<gapi_d3d12_cmd_list>(cmd_list);
}

t::shared_ptr<i::gapi_pipeline_layout> gapi_d3d12_device::create_pipeline_layout(const gapi_pipeline_layout_desc& desc)
{
	const auto d3d_desc = d3d_cast(desc);
	WinComPtr<ID3DBlob> content;
	WinComPtr<ID3DBlob> error;
	D3D12SerializeRootSignature(&d3d_desc, D3D_ROOT_SIGNATURE_VERSION_1, &content, &error);
	if (error.Get() != nullptr)
	{
		// TODO: LDA setup support
		WinComPtr<ID3D12RootSignature> signature;
		VERIFY(m_device->CreateRootSignature(0, content->GetBufferPointer(), content->GetBufferSize(), IID_PPV_ARGS(&signature)));
		return t::make_shared<gapi_d3d12_pipeline_layout>(signature);
	}
	return nullptr;
}

t::shared_ptr<i::gapi_pipeline_state> gapi_d3d12_device::create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC d3d_desc;
	d3d_desc.pRootSignature = nullptr;
	d3d_desc.CS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_compute_shader)->m_bytecode.Get());
	// TODO: LDA setup support
	d3d_desc.NodeMask = 0;
	d3d_desc.CachedPSO = {};
	d3d_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	
	WinComPtr<ID3D12PipelineState> pipeline_state;
	VERIFY(m_device->CreateComputePipelineState(&d3d_desc, IID_PPV_ARGS(&pipeline_state)));
	return t::make_shared<gapi_d3d12_pipeline_state>(pipeline_state, gapi_pipeline_state_type::compute);
}

t::shared_ptr<i::gapi_pipeline_state> gapi_d3d12_device::create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3d_desc = {};
	d3d_desc.pRootSignature = gapi_d3d12_pipeline_layout::cast(desc.m_pipeline_layout)->m_signature.Get();
	d3d_desc.VS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_vertex_shader)->m_bytecode.Get());
	d3d_desc.PS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_pixel_shader)->m_bytecode.Get());
	d3d_desc.DS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_domain_shader)->m_bytecode.Get());
	d3d_desc.HS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_hull_shader)->m_bytecode.Get());
	d3d_desc.GS = CD3DX12_SHADER_BYTECODE(gapi_d3d12_shader::cast(desc.m_bound_shader_state.m_geometry_shader)->m_bytecode.Get());
	// TODO: Vertex stream out support
	d3d_desc.StreamOutput = {};
	d3d_desc.BlendState = d3d_cast(desc.m_blend_state);
	d3d_desc.SampleMask = 0;
	d3d_desc.RasterizerState = d3d_cast(desc.m_rasterizer_state);
	d3d_desc.DepthStencilState = d3d_cast(desc.m_depth_stencil_state);
	d3d_desc.InputLayout = d3d_cast(desc.m_bound_shader_state.m_vertex_declaration);
	d3d_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	d3d_desc.PrimitiveTopologyType = d3d_cast(desc.m_primitive_type);
	d3d_desc.NumRenderTargets = static_cast<uint32>(desc.m_render_target_formats.size());
	for ( int32 i = 0; i < desc.m_render_target_formats.size(); ++i)
	{
		d3d_desc.RTVFormats[i] = d3d_cast(desc.m_render_target_formats[i]);
	}
	d3d_desc.DSVFormat = d3d_cast(desc.m_depth_stencil_format);
	d3d_desc.SampleDesc.Count = desc.m_num_samples;
	d3d_desc.SampleDesc.Quality = 0;
	// TODO: LDA setup support
	d3d_desc.NodeMask = 0;
	d3d_desc.CachedPSO = {};
	d3d_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	
	WinComPtr<ID3D12PipelineState> pipeline_state;
	VERIFY(m_device->CreateGraphicsPipelineState(&d3d_desc, IID_PPV_ARGS(&pipeline_state)));
	return t::make_shared<gapi_d3d12_pipeline_state>(pipeline_state, gapi_pipeline_state_type::graphics);
}

t::shared_ptr<i::gapi_descriptor_heap> gapi_d3d12_device::create_resource_view_heap(const gapi_descriptor_type& heap_type, const uint32& max_num_views)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3d_desc;
	
	d3d_desc.Type = d3d_cast(heap_type);
	d3d_desc.NumDescriptors = max_num_views;
	if (heap_type == gapi_descriptor_type::render_target_view || heap_type == gapi_descriptor_type::depth_stenicl_view)
	{
		d3d_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	
	}
	else
	{
		d3d_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	d3d_desc.NodeMask = 0;
	WinComPtr<ID3D12DescriptorHeap> heap;
	VERIFY(m_device->CreateDescriptorHeap(&d3d_desc, IID_PPV_ARGS(&heap)));
	uint32 descriptor_size = m_device->GetDescriptorHandleIncrementSize(d3d_desc.Type);
	return t::make_shared<gapi_d3d12_descriptor_heap>(heap, max_num_views, descriptor_size);
}

t::shared_ptr<i::gapi_constant_buffer_view> gapi_d3d12_device::create_constant_buffer_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_buffer>& buffer)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->is_created() == false);

	const auto& d3d_buffer = gapi_d3d12_buffer::cast(buffer);
	
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.BufferLocation = d3d_buffer->m_resource->GetGPUVirtualAddress();
	desc.SizeInBytes = d3d_buffer->get_resource_desc().m_width;
	
	m_device->CreateConstantBufferView(&desc, d3d_view->m_handle);

	auto result = t::make_shared<gapi_d3d12_constant_buffer_view>(t::exchange(d3d_view->m_index, 0), t::move(d3d_view->m_handle));
	result->m_created = true;
	return result;
}

t::shared_ptr<i::gapi_shader_resource_view> gapi_d3d12_device::create_shader_resource_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->m_created == false);
	
	const auto& d3d_resource = gapi_d3d12_resource::cast(resource);
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();

	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	
	switch (resource_desc.m_type)
	{
	case gapi_resource_type::buffer:
		{
			CHECK(t::has_any_flag(resource_desc.m_buffer_usage_flag, gapi_buffer_usage_flag::shader_resource));
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			// TODO: How to determine the structure layout?
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = 0;
			desc.Buffer.StructureByteStride = 0;
			break;	
		}
	case gapi_resource_type::texture1d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipLevels = resource_desc.m_num_mips;
			desc.Texture1D.MostDetailedMip = 1;
			desc.Texture1D.ResourceMinLODClamp = 1;
			break;	
		}
	case gapi_resource_type::texture2d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = resource_desc.m_num_mips;
			desc.Texture2D.MostDetailedMip = 1;
			desc.Texture2D.ResourceMinLODClamp = 1;
			break;
		}
	case gapi_resource_type::texture3d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			desc.Texture3D.MipLevels = resource_desc.m_num_mips;
			desc.Texture3D.MostDetailedMip = 1;
			desc.Texture3D.ResourceMinLODClamp = 1;
			break;	
		}
	default:
		CHECK(false);
	}
	m_device->CreateShaderResourceView(d3d_resource->m_resource.Get(), &desc, d3d_view->m_handle);
	auto result = t::make_shared<gapi_d3d12_shader_resource_view>(t::exchange(d3d_view->m_index, 0), t::move(d3d_view->m_handle));
	result->m_created = true;
	return result;
}

t::shared_ptr<i::gapi_unorder_access_view> gapi_d3d12_device::create_unordered_access_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_resource>& resource)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->m_created == false);
	
	const auto& d3d_resource = gapi_d3d12_resource::cast(resource);
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();

	D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
	
	switch (resource_desc.m_type)
	{
	case gapi_resource_type::buffer:
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
			// TODO: How to determine the structure layout?
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = 0;
			desc.Buffer.StructureByteStride = 0;
			break;
		}
	case gapi_resource_type::texture1d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipSlice = 0;
			break;
		}
	case gapi_resource_type::texture2d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;
			break;
		}
	case gapi_resource_type::texture3d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			desc.Texture3D.MipSlice = 0;
			desc.Texture3D.FirstWSlice = 0;
			desc.Texture3D.WSize = 0;
			break;
		}
	default:
			CHECK(false);
	}
	
	// TODO: Support UAV counter
	auto result = t::make_shared<gapi_d3d12_unorder_access_view>(t::exchange(d3d_view->m_index, 0), t::move(d3d_view->m_handle));
	result->m_created = true;
	return result;
}

t::shared_ptr<i::gapi_render_target_view> gapi_d3d12_device::create_render_target_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_texture>& texture)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->m_created == false);
	
	const auto& d3d_resource = gapi_d3d12_resource::cast(texture);
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();

	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	
	switch (resource_desc.m_type)
	{
	case gapi_resource_type::buffer:
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D12_RTV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = 0;
			break;
		}
	case gapi_resource_type::texture1d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			desc.Texture1D.MipSlice = 0;
			break;
		}
	case gapi_resource_type::texture2d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;
			break;
		}
	case gapi_resource_type::texture3d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			desc.Texture3D.MipSlice = 0;
			desc.Texture3D.FirstWSlice = 0;
			desc.Texture3D.WSize = 0;
			break;
		}
	default:
		CHECK(false);
	}
	//
	m_device->CreateRenderTargetView(d3d_resource->m_resource.Get(), &desc, d3d_view->m_handle);
	//
	auto result = t::make_shared<gapi_d3d12_render_target_view>(t::exchange(d3d_view->m_index, 0), t::move(d3d_view->m_handle));
	result->m_created = true;
	return result;
}

t::shared_ptr<i::gapi_depth_stencil_view> gapi_d3d12_device::create_depth_stencil_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_texture>& texture)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->m_created == false);
	
	const auto& d3d_resource = gapi_d3d12_resource::cast(texture);
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();

	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	
	switch (resource_desc.m_type)
	{
	case gapi_resource_type::texture1d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			desc.Flags = D3D12_DSV_FLAG_NONE;
			desc.Texture1D.MipSlice = 0;
			break;
		}
	case gapi_resource_type::texture2d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = D3D12_DSV_FLAG_NONE;
			desc.Texture2D.MipSlice = 0;
			break;
		}
	default:
		CHECK(false);
	}
	
	m_device->CreateDepthStencilView(d3d_resource->m_resource.Get(), &desc, d3d_view->m_handle);

	auto result = t::make_shared<gapi_d3d12_depth_stencil_view>(t::exchange(d3d_view->m_index, 0), t::move(d3d_view->m_handle));
	result->m_created = true;
	return result;
}

t::shared_ptr<i::gapi_sampler> gapi_d3d12_device::create_sampler(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const gapi_sampler_desc& desc)
{
	// CHECK(false);
	return nullptr;
	const auto& d3d_view = gapi_d3d12_descriptor::cast(allocated_view);
	CHECK(d3d_view->m_created == false);
	D3D12_SAMPLER_DESC d3d_desc = {};
	m_device->CreateSampler(&d3d_desc, d3d_view->m_handle);
	// return t::make_shared<gapi_d3d12_sampler>(d3d_view);
}

t::shared_ptr<i::gapi_resource_heap> gapi_d3d12_device::create_resource_heap()
{
	// TODO: Placed resouce heap creation
	CHECK(false);
	return nullptr;
}

t::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_resource(const gapi_resource_desc& desc)
{
	// TODO: Heap flag creation classify
	const CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_UPLOAD);
	const D3D12_RESOURCE_DESC states = d3d_cast(desc);
	WinComPtr<ID3D12Resource> resource;
	VERIFY(m_device->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&states,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource)
	));

	if (desc.m_type == gapi_resource_type::buffer)
	{
		return t::make_shared<gapi_d3d12_buffer>(resource, desc);
	}
	else
	{
		return t::make_shared<gapi_d3d12_texture>(resource, desc);
	}
}

t::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_placed_resource(const gapi_resource_desc& desc)
{
	// TODO: Placed resouce creation
	CHECK(false);
	return nullptr;
}

t::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_reserved_resource(const gapi_resource_desc& desc)
{
	// TODO: Reserved resouce heap creation
	CHECK(false);
	return nullptr;
}


