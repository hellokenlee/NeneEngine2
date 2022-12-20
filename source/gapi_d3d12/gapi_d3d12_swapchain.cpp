/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_swapchain.h"


gapi_d3d12_swapchain::gapi_d3d12_swapchain(t::shared_ptr<d3d12_adapter> adapter, HWND hwnd, uint32 back_buffer_num, uint32 multi_sample_num)
    : super()
    , m_last_fence_value(0)
    , m_back_buffer_index(0)
    , m_fence(nullptr)
    , m_swap_chain(nullptr)
{
    m_fence = t::make_shared<d3d12_fence>(adapter->get_device(0));
    m_swap_chain = t::make_shared<d3d12_swap_chain>(adapter, hwnd, back_buffer_num, multi_sample_num);
    m_back_buffer_index = m_swap_chain->get_current_back_buffer_index();
}

void gapi_d3d12_swapchain::start_frame()
{
    // Wait for last submitted frame
    m_fence->wait(m_last_fence_value);

    //
    m_back_buffer_index = m_swap_chain->get_current_back_buffer_index();
}

void gapi_d3d12_swapchain::finish_frame()
{
    //
    m_swap_chain->present();
    //
    m_last_fence_value = m_fence->signal();
}

t::shared_ptr<gapi_texture> gapi_d3d12_swapchain::get_back_buffer_texture() const
{
    return gapi_d3d12_texture_2d::wrap(m_swap_chain->get_back_buffer_texture(m_back_buffer_index));
}
