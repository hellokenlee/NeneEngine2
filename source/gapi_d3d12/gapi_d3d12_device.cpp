/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_device.h"

#include "gapi_d3d12_cmd_allocator.h"
#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_resource_heap.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_resource_view.h"
#include "gapi_d3d12_resource_view_allocator.h"
#include "gapi_d3d12_shader.h"

#include "d3d12_type_cast.h"
#include "d3d12_root_signature_manager.h"

extern t::console_var<bool> cvar_gapi_d3d_debug;
extern logger d3d12_;
static gapi_d3d12_device* g_gapi_d3d12_device = nullptr;

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static LONG __stdcall d3d_vectored_exception_handler(EXCEPTION_POINTERS* info)
{
	if (info->ExceptionRecord->ExceptionCode == _FACDXGI)
	{
		if (g_gapi_d3d12_device != nullptr)
		{
			g_gapi_d3d12_device->print_d3d_debug_messages();
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void gapi_d3d12_device::print_d3d_debug_messages() const
{
	size_t num_message = m_d3d_debug_info_queue->GetNumStoredMessagesAllowedByRetrievalFilter();
	for (size_t i = 0; i < num_message; i++)
	{
		size_t message_length = 0;
		VERIFY(m_d3d_debug_info_queue->GetMessage(i, nullptr, &message_length));

		D3D12_MESSAGE* d3d_message = nullptr;
		d3d_message = static_cast<D3D12_MESSAGE*>(malloc(message_length));

		if (d3d_message != nullptr)
		{
			VERIFY(m_d3d_debug_info_queue->GetMessage(i, d3d_message, &message_length));
			log(d3d12_, error, "D3D12 DebugLayer: {}", std::string_view(d3d_message->pDescription, d3d_message->DescriptionByteLength));
			free(d3d_message);
		}
	}
}

gapi_d3d12_device::gapi_d3d12_device(const WinComPtr<ID3D12Device>& device)
	: m_d3d_device(device)
	, m_d3d_device2(nullptr)
{
	//
	m_d3d_device->QueryInterface(IID_PPV_ARGS(&m_d3d_device2));
	// filter debug message
	if (cvar_gapi_d3d_debug.value())
	{
		if (SUCCEEDED(m_d3d_device2.As(&m_d3d_debug_info_queue)))
		{
			m_d3d_debug_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			m_d3d_debug_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			m_d3d_debug_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

			// supress some warnings
			D3D12_MESSAGE_CATEGORY categories_to_supress[] = {
				D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED
			};
			D3D12_MESSAGE_SEVERITY severities_to_supress[] = {
				D3D12_MESSAGE_SEVERITY_INFO
			};
			D3D12_MESSAGE_ID message_to_supress[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER info_queue_filter = {};
			info_queue_filter.DenyList.NumCategories = _countof(categories_to_supress);
			info_queue_filter.DenyList.pCategoryList = categories_to_supress;
			info_queue_filter.DenyList.NumSeverities = _countof(severities_to_supress);
			info_queue_filter.DenyList.pSeverityList = severities_to_supress;
			info_queue_filter.DenyList.NumIDs = _countof(message_to_supress);
			info_queue_filter.DenyList.pIDList = message_to_supress;
			VERIFY(m_d3d_debug_info_queue->PushStorageFilter(&info_queue_filter));
		}
		//
		CHECK(g_gapi_d3d12_device == nullptr);
		g_gapi_d3d12_device = this;
		m_debug_exception_handler = AddVectoredExceptionHandler(0, d3d_vectored_exception_handler);
	}
	//
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::graphics)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::graphics);
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::compute)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::compute);
	m_cmd_queues[static_cast<uint32>(gapi_cmd_type::copy)] = gapi_d3d12_device::create_cmd_queue(gapi_cmd_type::copy);

	//
	// check resource tiers
	// refs: https://microsoft.github.io/DirectX-Specs/d3d/ResourceBinding.html#Levels%20of%20Hardware%20Support
	D3D12_FEATURE_DATA_D3D12_OPTIONS d3d_options{};
	m_d3d_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &d3d_options, sizeof(d3d_options));
	m_d3d_resource_binding_tier = d3d_options.ResourceBindingTier;
	m_d3d_resource_heap_tier = d3d_options.ResourceHeapTier;
	// nene require at least tier 2
	CHECK(m_d3d_resource_binding_tier >= D3D12_RESOURCE_BINDING_TIER_2);
}

std::shared_ptr<i::gapi_cmd_queue> gapi_d3d12_device::create_cmd_queue(gapi_cmd_type cmd_type)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = d3d_cast(cmd_type);

	WinComPtr<ID3D12CommandQueue> cmd_queue;
	VERIFY(m_d3d_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&cmd_queue)));

	WinComPtr<ID3D12Fence> fence;
	VERIFY(m_d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	
	return std::make_shared<gapi_d3d12_cmd_queue>(cmd_type, cmd_queue, fence);
}

std::shared_ptr<i::gapi_cmd_allocator> gapi_d3d12_device::create_cmd_allocator(gapi_cmd_type cmd_type)
{
	WinComPtr<ID3D12CommandAllocator> allocator;
	VERIFY(m_d3d_device->CreateCommandAllocator(d3d_cast(cmd_type), IID_PPV_ARGS(&allocator)));

	return std::make_shared<gapi_d3d12_cmd_allocator>(allocator);
}

std::shared_ptr<i::gapi_cmd_list> gapi_d3d12_device::create_cmd_list(gapi_cmd_type type, std::shared_ptr<i::gapi_cmd_allocator>& allocator)
{
	WinComPtr<ID3D12GraphicsCommandList> d3d_cmd_list;
	VERIFY(m_d3d_device->CreateCommandList(0, d3d_cast(type), t::gapi_pin<gapi_d3d12_cmd_allocator>(allocator).get_d3d_allocator(), nullptr, IID_PPV_ARGS(&d3d_cmd_list)));
	d3d_cmd_list->Close();
	return std::make_shared<gapi_d3d12_cmd_list>(d3d_cmd_list);
}

std::shared_ptr<i::gapi_pipeline_state> gapi_d3d12_device::create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc)
{
	//
	D3D12_COMPUTE_PIPELINE_STATE_DESC d3d_desc;
	d3d_desc.pRootSignature = nullptr;
	d3d_desc.CS = CD3DX12_SHADER_BYTECODE(t::gapi_pin<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::compute_shader)).get_d3d_bytecode());
	// TODO: LDA setup support
	d3d_desc.NodeMask = 0;
	d3d_desc.CachedPSO = {};
	d3d_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	
	WinComPtr<ID3D12PipelineState> d3d_pipeline_state;
	VERIFY(m_d3d_device->CreateComputePipelineState(&d3d_desc, IID_PPV_ARGS(&d3d_pipeline_state)));
	return std::make_shared<gapi_d3d12_pipeline_state>(std::move(d3d_pipeline_state), gapi_pipeline_state_type::compute, nullptr);
}

std::shared_ptr<i::gapi_pipeline_state> gapi_d3d12_device::create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc)
{
	//
	auto d3d_root_signature = d3d12_root_signature_manager::get().find_or_create_root_signature(get_d3d_device(), desc.m_bound_shader_state);
	std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs;
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3d_desc = {};
	//
	d3d_desc.pRootSignature = d3d_root_signature.Get();
	//
	d3d_desc.VS = CD3DX12_SHADER_BYTECODE(t::gapi_pin<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::vertex_shader)).get_d3d_bytecode());
	//
	if (auto pixel_shader = t::gapi_cast<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::pixel_shader)); pixel_shader != nullptr)
	{
		d3d_desc.PS = CD3DX12_SHADER_BYTECODE(pixel_shader->get_d3d_bytecode());
	}
	if (auto domain_shader = t::gapi_cast<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::domain_shader)); domain_shader != nullptr)
	{
		d3d_desc.DS = CD3DX12_SHADER_BYTECODE(domain_shader->get_d3d_bytecode());
	}
	if (auto hull_shader = t::gapi_cast<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::hull_shader)); hull_shader != nullptr)
	{
		d3d_desc.HS = CD3DX12_SHADER_BYTECODE(hull_shader->get_d3d_bytecode());
	}
	if (auto geometry_shader = t::gapi_cast<gapi_d3d12_shader>(desc.m_bound_shader_state.get_shader(gapi_shader_stage::geometry_shader)); geometry_shader != nullptr)
	{
		d3d_desc.GS = CD3DX12_SHADER_BYTECODE(geometry_shader->get_d3d_bytecode());
	}
	// TODO: Vertex stream out support
	d3d_desc.StreamOutput = {};
	d3d_desc.BlendState = d3d_cast(desc.m_blend_state);
	d3d_desc.SampleMask = UINT_MAX;
	d3d_desc.RasterizerState = d3d_cast(desc.m_rasterizer_state);
	d3d_desc.DepthStencilState = d3d_cast(desc.m_depth_stencil_state);
	d3d_desc.InputLayout = d3d_cast(desc.m_bound_shader_state.get_vertices_declaration(), input_element_descs);
	d3d_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	d3d_desc.PrimitiveTopologyType = d3d_cast(desc.m_primitive_type);
	d3d_desc.NumRenderTargets = static_cast<uint32>(desc.m_render_target_formats.size());
	for (int32 i = 0; i < desc.m_render_target_formats.size(); ++i)
	{
		d3d_desc.RTVFormats[i] = d3d_cast(desc.m_render_target_formats[i]);
	}
	d3d_desc.DSVFormat = d3d_cast(desc.m_depth_stencil_format);
	d3d_desc.SampleDesc.Count = desc.m_num_samples;
	d3d_desc.SampleDesc.Quality = 0;
	// TODO: LDA setup support
	d3d_desc.NodeMask = 0;
	// TODO: PSO caching
	d3d_desc.CachedPSO = {};
	d3d_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	
	WinComPtr<ID3D12PipelineState> d3d_pipeline_state;
	VERIFY(m_d3d_device->CreateGraphicsPipelineState(&d3d_desc, IID_PPV_ARGS(&d3d_pipeline_state)));
	return std::make_shared<gapi_d3d12_pipeline_state>(std::move(d3d_pipeline_state), gapi_pipeline_state_type::graphics, std::move(d3d_root_signature));
}

std::shared_ptr<i::gapi_resource_view_allocator> gapi_d3d12_device::create_resource_view_allocator(gapi_resource_view_type view_type, gapi_resource_view_allocator_type allocator_type, const uint32& max_num_views)
{
	// This method always create offline descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC d3d_desc;
	
	d3d_desc.Type = d3d_cast(view_type);
	d3d_desc.NumDescriptors = max_num_views;
	d3d_desc.Flags = allocator_type == gapi_resource_view_allocator_type::offline ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	d3d_desc.NodeMask = 0;
	WinComPtr<ID3D12DescriptorHeap> heap;
	VERIFY(m_d3d_device->CreateDescriptorHeap(&d3d_desc, IID_PPV_ARGS(&heap)));
	// query the size of different descriptor because it's vendor specific
	uint32 descriptor_size = m_d3d_device->GetDescriptorHandleIncrementSize(d3d_desc.Type);
	return std::make_shared<gapi_d3d12_resource_view_allocator>(heap, d3d_desc, max_num_views, descriptor_size);
}

void gapi_d3d12_device::create_constant_buffer_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_buffer>& buffer)
{
	//
	CHECK(allocated_view->get_type() == gapi_resource_view_type::unknown);
	//
	const auto& d3d_buffer = t::gapi_cast<gapi_d3d12_buffer>(buffer);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	
	//
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.BufferLocation = d3d_buffer->get_d3d_resource()->GetGPUVirtualAddress();
	desc.SizeInBytes = d3d_buffer->get_resource_desc().m_width;
	//
	m_d3d_device->CreateConstantBufferView(&desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::constant_buffer_view);
}

void gapi_d3d12_device::create_shader_resource_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_resource>& resource)
{
	//
	CHECK(allocated_view->get_type() == gapi_resource_view_type::unknown);
	//
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_resource>(resource);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();
	//
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//
	switch (resource_desc.m_type)
	{
	case gapi_resource_type::buffer:
		{
			CHECK(t::has_flag(resource_desc.m_buffer_usage_flag, gapi_buffer_usage_flag::shader_resource));
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
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
			desc.Texture1D.MostDetailedMip = 0;
			desc.Texture1D.ResourceMinLODClamp = 1;
			break;	
		}
	case gapi_resource_type::texture2d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = resource_desc.m_num_mips;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.ResourceMinLODClamp = 1;
			break;
		}
	case gapi_resource_type::texture3d:
		{
			desc.Format = d3d_cast(resource_desc.m_format);
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			desc.Texture3D.MipLevels = resource_desc.m_num_mips;
			desc.Texture3D.MostDetailedMip = 0;
			desc.Texture3D.ResourceMinLODClamp = 1;
			break;	
		}
	default:
		CHECK(false);
	}
	//
	m_d3d_device->CreateShaderResourceView(d3d_resource->get_d3d_resource(), &desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::shader_resource_view);
}

void gapi_d3d12_device::create_unordered_access_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_resource>& resource)
{
	//
	CHECK(allocated_view->get_type() == gapi_resource_view_type::unknown);
	//
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_resource>(resource);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	//
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();
	//
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
	m_d3d_device->CreateUnorderedAccessView(d3d_resource->get_d3d_resource(), nullptr, &desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::unordered_access_view);
}

void gapi_d3d12_device::create_render_target_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_texture>& texture)
{
	//
	CHECK(allocated_view->get_type() == gapi_resource_view_type::unknown);
	//
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_texture>(texture);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	//
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();
	//
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
	m_d3d_device->CreateRenderTargetView(d3d_resource->get_d3d_resource(), &desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::render_target_view);
}

void gapi_d3d12_device::create_depth_stencil_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_texture>& texture)
{
	//
	CHECK(allocated_view->get_type() == gapi_resource_view_type::unknown);
	//
	const auto& d3d_resource = t::gapi_cast<gapi_d3d12_texture>(texture);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	//
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	const gapi_resource_desc& resource_desc = d3d_resource->get_resource_desc();
	//
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
	//
	m_d3d_device->CreateDepthStencilView(d3d_resource->get_d3d_resource(), &desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::depth_stencil_view);
}

void gapi_d3d12_device::create_sampler(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const gapi_sampler_desc& desc)
{
	//
	const auto d3d_desc = d3d_cast(desc);
	auto& d3d_view = t::gapi_pin<gapi_d3d12_offline_resource_view>(allocated_view);
	//
	m_d3d_device->CreateSampler(&d3d_desc, d3d_view.get_d3d_cpu_handle());
	d3d_view.set_type(gapi_resource_view_type::texture_sampler);
}

std::shared_ptr<i::gapi_resource_allocator> gapi_d3d12_device::create_resource_allocator()
{
	NOT_IMPLEMENTED();
	return nullptr;
}

std::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_resource(const gapi_resource_desc& desc)
{
	// refs: https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_heap_type
	// UE5: D3D12Texture.cpp::FD3D12DynamicRHI::CreateD3D12Texture(...), D3D12Buffer.cpp::FD3D12DynamicRHI::CreateD3D12Buffer(...)

	// choose a heap
	D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT;
	if (desc.is_texture() && t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::cpu_readable))
	{
		heap_type = D3D12_HEAP_TYPE_READBACK;
	}
	else if (desc.is_buffer() && t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::dynamic_buffer))
	{
		heap_type = D3D12_HEAP_TYPE_UPLOAD;
	}

	// TODO: choose initial resource state
	const CD3DX12_HEAP_PROPERTIES d3d_heap_props(heap_type);
	const D3D12_RESOURCE_DESC d3d_desc = d3d_cast(desc);
	WinComPtr<ID3D12Resource> resource;
	VERIFY(m_d3d_device->CreateCommittedResource(
		&d3d_heap_props,
		D3D12_HEAP_FLAG_NONE,
		&d3d_desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&resource)
	));

	if (desc.m_type == gapi_resource_type::buffer)
	{
		return std::make_shared<gapi_d3d12_buffer>(resource, desc);
	}
	else
	{
		return std::make_shared<gapi_d3d12_texture>(resource, desc);
	}
}

std::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_placed_resource(const gapi_resource_desc& desc)
{
	NOT_IMPLEMENTED();
	return nullptr;
}

std::shared_ptr<i::gapi_resource> gapi_d3d12_device::create_reserved_resource(const gapi_resource_desc& desc)
{
	NOT_IMPLEMENTED();
	return nullptr;
}

std::shared_ptr<i::gapi_shader> gapi_d3d12_device::create_and_compile_shader(const gapi_shader_stage& stype, const std::string& source, const std::string& entry, const gapi_shader_feature_level& level, const std::string& debug_name)
{
	auto result = std::make_shared<gapi_d3d12_shader>(stype, level, source, entry, debug_name);
	if (result->compile())
	{
		return result;
	}
	return nullptr;
}
