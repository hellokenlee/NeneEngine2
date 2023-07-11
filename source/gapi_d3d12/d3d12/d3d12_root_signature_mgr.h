/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_root_signature.h"
#include "d3d12_pipeline_state.h"


class d3d12_root_signature_mgr : public d3d12_device_child
{
public:
    d3d12_root_signature_mgr(t::shared_ptr<d3d12_device> device);

    t::shared_ptr<d3d12_root_signature> obtain_root_signature(const d3d12_graphics_pipeline_creation_args& pipeline_creation_args);

public:
    t::map<uint64, t::shared_ptr<d3d12_root_signature>> m_root_signature_map;
};


class d3d12_root_signature_desc
{
public:
    d3d12_root_signature_desc(const d3d12_graphics_pipeline_creation_args& pipeline_creation_args);

    [[nodiscard]] const CD3DX12_ROOT_SIGNATURE_DESC& get_d3d_desc() const { return m_desc; }
    
protected:
    t::dynamic_array<CD3DX12_DESCRIPTOR_RANGE> m_descriptor_table_items;
    t::dynamic_array<CD3DX12_ROOT_PARAMETER> m_descriptor_tables;

    CD3DX12_ROOT_SIGNATURE_DESC m_desc;
};
