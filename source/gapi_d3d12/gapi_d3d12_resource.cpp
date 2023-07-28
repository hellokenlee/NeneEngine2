/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"


gapi_d3d12_resource::gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
	: m_desc(desc)
	, m_resource(resource)
{
}