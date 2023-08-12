/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"
#include "d3d12_utils.h"


class gapi_d3d12_resource : public t::impl<gapi_d3d12_resource, i::gapi_resource>
{
public:
	gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept
		: super()
		, m_desc(t::move(other.m_desc))
		, m_resource(t::move(m_resource))
	{}
	~gapi_d3d12_resource() override = default;

	const gapi_resource_desc& get_resource_desc() override { return m_desc; };

public:
	gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);

	ID3D12Resource* get_d3d_resource() const { return m_resource.Get(); }

protected:
	gapi_resource_desc m_desc;
	WinComPtr<ID3D12Resource> m_resource;
};


class gapi_d3d12_texture : public t::poly_impl<gapi_d3d12_texture, gapi_d3d12_resource, i::gapi_texture>
{
public:
	using super::super;

	~gapi_d3d12_texture() override = default;
};


class gapi_d3d12_buffer : public t::poly_impl<gapi_d3d12_buffer, gapi_d3d12_resource, i::gapi_buffer>
{
public:
	using super::super;
	
	~gapi_d3d12_buffer() override = default;
};
