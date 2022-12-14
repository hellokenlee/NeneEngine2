/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_d3d12_texture.h"
#include "gapi/gapi_template.h"
#include "gapi/gapi_swapchain.h"

#include "d3d12/d3d12_fence.h"
#include "d3d12/d3d12_adapter.h"
#include "d3d12/d3d12_texture.h"
#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_swap_chain.h"


class gapi_d3d12_swapchain : public t::impl<gapi_d3d12_swapchain, gapi_swapchain>
{
public:
    gapi_d3d12_swapchain(t::shared_ptr<d3d12_adapter> adapter, HWND hwnd, uint32 back_buffer_num, uint32 multi_sample_num);
    ~gapi_d3d12_swapchain() override = default;

public:
    void start_frame();

    void finish_frame();

    virtual t::shared_ptr<gapi_texture> get_back_buffer_texture() const override;

public:
    //
    uint64 m_last_fence_value;
    uint32 m_back_buffer_index;
    //
    t::shared_ptr<d3d12_fence> m_fence;
    t::shared_ptr<d3d12_swap_chain> m_swap_chain;
};