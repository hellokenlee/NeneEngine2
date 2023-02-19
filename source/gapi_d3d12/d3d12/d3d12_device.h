/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>

#include "d3d12_utils.h"
#include "d3d12_adapter.h"

class d3d12_descriptor_heap;
class d3d12_cmd_list_mgr;
class d3d12_root_signature_mgr;


class d3d12_device : public noncopyable, public t::enable_shared_from_this<d3d12_device>, public d3d12_adapter_child
{
public:
	d3d12_device() = delete;
	d3d12_device(t::shared_ptr<d3d12_adapter> adapter);
	d3d12_device(t::shared_ptr<d3d12_adapter> adapter, ID3D12Device* d3d_device);
	
	~d3d12_device() override;

	void init();
	void clear();

	[[nodiscard]] ID3D12Device* get_d3d_device() const { return m_device.Get(); }

	[[nodiscard]] t::shared_ptr<d3d12_descriptor_heap> get_rtv_descriptor_heap() const { return m_rtv_descriptor_heap; }
	[[nodiscard]] t::shared_ptr<d3d12_descriptor_heap> get_dsv_descriptor_heap() const { return m_dsv_descriptor_heap; }
	[[nodiscard]] t::shared_ptr<d3d12_descriptor_heap> get_global_descriptor_heap() const { return m_global_descriptor_heap; }

	[[nodiscard]] t::shared_ptr<d3d12_cmd_list_mgr> get_cmd_list_mgr(d3d12_cmd_type type) const;
	[[nodiscard]] t::shared_ptr<d3d12_cmd_list_mgr> get_copy_cmd_list_mgr() const { return m_copy_cmd_list_mgr; }
	[[nodiscard]] t::shared_ptr<d3d12_cmd_list_mgr> get_compute_cmd_list_mgr() const { return m_compute_cmd_list_mgr; }
	[[nodiscard]] t::shared_ptr<d3d12_cmd_list_mgr> get_graphics_cmd_list_mgr() const { return m_graphics_cmd_list_mgr; }

	[[nodiscard]] t::shared_ptr<d3d12_root_signature_mgr> get_root_signature_mgr() const { return m_root_signature_mgr; }
	
protected:
	// Descriptor allocators
	t::shared_ptr<d3d12_descriptor_heap> m_rtv_descriptor_heap;
	t::shared_ptr<d3d12_descriptor_heap> m_dsv_descriptor_heap;
	t::shared_ptr<d3d12_descriptor_heap> m_global_descriptor_heap;

	// Command list managers
	t::shared_ptr<d3d12_cmd_list_mgr> m_copy_cmd_list_mgr;
	t::shared_ptr<d3d12_cmd_list_mgr> m_compute_cmd_list_mgr;
	t::shared_ptr<d3d12_cmd_list_mgr> m_graphics_cmd_list_mgr;

	// Root signature manager
	t::shared_ptr<d3d12_root_signature_mgr> m_root_signature_mgr;

	// Raw device pointer
	WinComPtr<ID3D12Device> m_device;
};


class d3d12_device_child
{
public:
	d3d12_device_child() = delete;

	explicit d3d12_device_child(t::shared_ptr<d3d12_device> device) : m_device(device) {}

	virtual ~d3d12_device_child() = default;

	t::shared_ptr<d3d12_device> get_parent_device() { return m_device; }

protected:
	t::shared_ptr<d3d12_device> m_device{};
};
