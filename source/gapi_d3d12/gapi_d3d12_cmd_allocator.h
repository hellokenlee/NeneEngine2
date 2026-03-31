/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_allocator.h"
#include "d3d12_utils.h"


namespace nene
{
	class gapi_d3d12_cmd_allocator : public gapi_cmd_allocator
	{
	public:
		~gapi_d3d12_cmd_allocator() override = default;

		void reset() override;
		
		void set_debug_name(const std::wstring& debug_name) override { d3d_set_debug_name(*get_d3d_allocator(), debug_name); }
		
	public:
		gapi_d3d12_cmd_allocator(const WinComPtr<ID3D12CommandAllocator>& allocator);

		ID3D12CommandAllocator* get_d3d_allocator() const { return m_allocator.Get(); }
		
	private:
		WinComPtr<ID3D12CommandAllocator> m_allocator;
	};
}
