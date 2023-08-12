/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_layout.h"


gapi_d3d12_pipeline_layout::gapi_d3d12_pipeline_layout(const WinComPtr<ID3D12RootSignature>& signature)
	: m_signature(signature)
{
}