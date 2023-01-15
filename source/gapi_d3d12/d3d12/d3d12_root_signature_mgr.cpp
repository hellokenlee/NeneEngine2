/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_root_signature_mgr.h"

#define D3D12_PIPELINE_CREATION_HASH 0


d3d12_root_signature_mgr::d3d12_root_signature_mgr(t::shared_ptr<d3d12_device> device)
    : d3d12_device_child(device)
{
}

t::shared_ptr<d3d12_root_signature> d3d12_root_signature_mgr::obtain_root_signature(const d3d12_graphics_pipeline_creation_args pipeline_args)
{
    // TODO: Pipeline hasing
#if D3D12_PIPELINE_CREATION_HASH
    const uint64 pipeline_state_hash = t::hash<d3d12_graphics_pipeline_creation_args>{}();

    // Existing root signature
    if (const auto it = m_root_signature_map.find(pipeline_state_hash); it == m_root_signature_map.end())
    {
        return it->second;
    }
    */
#else
    static uint64 pipeline_state_hash = 1;
    pipeline_state_hash += 1;
#endif
    
    // Create a new root signature
    auto new_root_signature = t::make_shared<d3d12_root_signature>(get_parent_device());
    m_root_signature_map.insert(t::make_pair(pipeline_state_hash, new_root_signature));
    return new_root_signature;
}