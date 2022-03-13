/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "core/core.h"
#include "d3d12_types.h"


class d3d12_device;


class d3d12_adapter : private noncopyable
{
public:
	static shared_ptr<d3d12_adapter> select_adapter();

public:
	[[nodiscard]]
	IDXGIAdapter* get_dxgi_adapter() const { return m_adapter.Get(); }

	[[nodiscard]]
	IDXGIFactory* get_dxgi_factory() const { return m_factory.Get(); }

	[[nodiscard]]
	IDXGIFactory6* get_dxgi_factory6() const { return m_factory.Get(); }
	
	shared_ptr<d3d12_device> get_device(const uint32 index);

	uint32 append_device(const shared_ptr<d3d12_device>& device);

	void remove_all_devices();

private:
	dynamic_array<shared_ptr<d3d12_device>> m_devices;

private:
	WinComPtr<IDXGIAdapter> m_adapter;
	WinComPtr<IDXGIFactory6> m_factory;
};


class d3d12_adapter_child
{
public:
	d3d12_adapter_child() = delete;

	d3d12_adapter_child(shared_ptr<d3d12_adapter> adapter) : m_adapter(adapter) {}

	~d3d12_adapter_child() = default;

	shared_ptr<d3d12_adapter> get_parent_adapter() { return m_adapter; }

protected:
	shared_ptr<d3d12_adapter> m_adapter;
};
