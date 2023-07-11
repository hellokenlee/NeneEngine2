/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_root_signature_mgr.h"

#define D3D12_PIPELINE_CREATION_HASH 0


d3d12_root_signature_mgr::d3d12_root_signature_mgr(t::shared_ptr<d3d12_device> device)
    : d3d12_device_child(device)
{
}

t::shared_ptr<d3d12_root_signature> d3d12_root_signature_mgr::obtain_root_signature(const d3d12_graphics_pipeline_creation_args& pipeline_args)
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

d3d12_root_signature_desc::d3d12_root_signature_desc(const d3d12_graphics_pipeline_creation_args& pipeline_creation_args)
    : m_desc()
{
    // TODO: Merge descriptor tables if possible
    // Currently each shader stage has its one descriptor table.
    // In each descriptor table, one item for each descriptor type.
    // In each descriptor table item contains all descriptors the shader stage is used.

    auto create_descriptor_table = [](const d3d12_shader_resource_table& resource_table, D3D12_SHADER_VISIBILITY visibility)
    {
        for (int i = 0; i < resource_table.size(); ++i)
        {
            CD3DX12_DESCRIPTOR_RANGE descriptor_table_items[NUM_DESCRIPTOR_TABLE_ITEMS];
        }
    };

    
    // Vertex Shader
    {
        
    }

    constexpr uint32 NUM_DESCRIPTOR_TABLE_ITEMS = 1;
    constexpr uint32 NUM_DESCRIPTOR_TABLE = 1;

    // Each item in descriptor table is a range in descriptor heap
   
    //

    m_desc.Init(
              1, root_parameters,
              static_cast<uint32>(shared_static_sampler_descs.size()), shared_static_sampler_descs.data(),
              D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
           );

    CD3DX12_DESCRIPTOR_RANGE descriptor_table_items[NUM_DESCRIPTOR_TABLE_ITEMS];
    descriptor_table_items[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER root_parameters[NUM_DESCRIPTOR_TABLE];
    root_parameters[0].InitAsDescriptorTable(1, descriptor_table_items, D3D12_SHADER_VISIBILITY_PIXEL);
    
       
}