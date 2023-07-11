/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"
#include "d3d12_device.h"
#include "d3d12_root_signature_mgr.h"


class d3d12_root_signature : public d3d12_device_child
{
public:
    d3d12_root_signature(t::shared_ptr<d3d12_device> device, const d3d12_root_signature_desc& desc);

    [[nodiscard]] ID3D12RootSignature* get_d3d_root_signature() const { return m_root_signature.Get(); }
    
protected:
    WinComPtr<ID3D12RootSignature> m_root_signature;
};