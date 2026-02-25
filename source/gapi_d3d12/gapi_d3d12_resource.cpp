/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"
#include "d3d12_type_cast.h"
#include "core/utils/string_utils.h"


namespace nene
{
	gapi_d3d12_resource::gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
		: gapi_resource(desc)
		, m_d3d_resource(resource)
	{
		m_desc = desc;
	}

	gapi_d3d12_resource::gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept
		: gapi_resource(std::move(other))
		, m_d3d_resource(std::move(other.m_d3d_resource))
	{
		m_d3d_resource = std::move(other.m_d3d_resource);
	}

	void gapi_d3d12_resource::map(const upoint64_t& read_range, std::function<void(void*)> vram_operator)
	{
		D3D12_RANGE _read_range = {read_range.begin, read_range.end};
		void* mapped_memory = nullptr;
		m_d3d_resource->Map(0, &_read_range, &mapped_memory);
		vram_operator(mapped_memory);
		m_d3d_resource->Unmap(0, nullptr);
	}

	void gapi_d3d12_resource::set_debug_name(const std::wstring& debug_name)
	{
		d3d_set_debug_name(*m_d3d_resource.Get(), debug_name);
	}

	std::optional<CD3DX12_RESOURCE_BARRIER> gapi_d3d12_resource::d3d_transition(const gapi_resource_state& to_state)
	{
		if (m_state == to_state)
		{
			return std::nullopt;
		}
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			get_d3d_resource(),
			d3d_cast(m_state),
			d3d_cast(to_state)
		);
		m_state = to_state;
		return barrier;
	}

	gapi_d3d12_texture::gapi_d3d12_texture(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
		: gapi_resource(desc)
		, gapi_d3d12_resource(resource, desc)
		, gapi_texture(desc)
	{
		gapi_d3d12_resource::set_debug_name(utils::string_to_wstring(desc.m_debug_name));
	}

	gapi_d3d12_buffer::gapi_d3d12_buffer(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
		: gapi_resource(desc)
		, gapi_d3d12_resource(resource, desc)
		, gapi_buffer(desc)
	{
		//
		CHECK(desc.is_buffer());
		//
		m_optional_index_buffer_view = {};
		m_optional_vertex_buffer_view = {};
		if (is_index_buffer())
		{
			CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer))
			m_optional_index_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
			m_optional_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
			m_optional_index_buffer_view.SizeInBytes = static_cast<uint32_t>(desc.buffer_size());
		}
		else if (is_vertex_buffer())
		{
			CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer))
			m_optional_vertex_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
			m_optional_vertex_buffer_view.SizeInBytes = static_cast<uint32_t>(desc.buffer_size());
			m_optional_vertex_buffer_view.StrideInBytes = desc.m_vertex_buffer_stride;
		}
		//
		gapi_d3d12_resource::set_debug_name(utils::string_to_wstring(desc.m_debug_name));
	}
}
