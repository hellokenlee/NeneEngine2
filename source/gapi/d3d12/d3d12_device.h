/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "d3d12_utils.h"
#include "d3d12_adapter.h"

class d3d12_descriptor_heap;
class d3d12_cmd_list_mgr;


class d3d12_device : public noncopyable, public enable_shared_from_this<d3d12_device>, public d3d12_adapter_child
{
public:
	d3d12_device() = delete;

	d3d12_device(shared_ptr<d3d12_adapter> adapter);

	virtual void init();

	ID3D12Device* get_d3d_device() const { return m_device.Get(); }

	shared_ptr<d3d12_descriptor_heap> get_rtv_descriptor_heap() const { return m_rtv_descriptor_heap; }
	shared_ptr<d3d12_descriptor_heap> get_dsv_descriptor_heap() const { return m_dsv_descriptor_heap; }
	shared_ptr<d3d12_descriptor_heap> get_global_descriptor_heap() const { return m_global_descriptor_heap; }

	shared_ptr<d3d12_cmd_list_mgr> get_cmd_list_mgr(d3d12_cmd_type type);
	shared_ptr<d3d12_cmd_list_mgr> get_copy_cmd_list_mgr() const { return m_copy_cmd_list_mgr; }
	shared_ptr<d3d12_cmd_list_mgr> get_compute_cmd_list_mgr() const { return m_compute_cmd_list_mgr; }
	shared_ptr<d3d12_cmd_list_mgr> get_graphics_cmd_list_mgr() const { return m_graphics_cmd_list_mgr; }

public:
	// Descriptor allocators
	shared_ptr<d3d12_descriptor_heap> m_rtv_descriptor_heap;
	shared_ptr<d3d12_descriptor_heap> m_dsv_descriptor_heap;
	shared_ptr<d3d12_descriptor_heap> m_global_descriptor_heap;

	// Command list managers
	shared_ptr<d3d12_cmd_list_mgr> m_copy_cmd_list_mgr;
	shared_ptr<d3d12_cmd_list_mgr> m_compute_cmd_list_mgr;
	shared_ptr<d3d12_cmd_list_mgr> m_graphics_cmd_list_mgr;

protected:
	WinComPtr<ID3D12Device> m_device;
};


class d3d12_device_child
{
public:
	d3d12_device_child() = delete;

	d3d12_device_child(shared_ptr<d3d12_device> device) : m_device(device) {}

	~d3d12_device_child() = default;

	shared_ptr<d3d12_device> get_parent_device() { return m_device; }

protected:
	shared_ptr<d3d12_device> m_device;
};
