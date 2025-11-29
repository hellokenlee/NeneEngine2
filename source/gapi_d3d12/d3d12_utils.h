/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi.h"

#include <Windows.h>
#undef GetMessage
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include "d3dx12.h"
#include "d3d12_limits.h"


inline std::string get_error_string(const HRESULT code)
{
	//
	std::string res;
	//
#define CASE_WINDOWS_ERROR(x) case x: res = #x; break;
	//
	switch (code)
	{
		CASE_WINDOWS_ERROR(S_OK);
		CASE_WINDOWS_ERROR(D3D11_ERROR_FILE_NOT_FOUND)
		CASE_WINDOWS_ERROR(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
		CASE_WINDOWS_ERROR(E_FAIL)
		CASE_WINDOWS_ERROR(E_INVALIDARG)
		CASE_WINDOWS_ERROR(E_OUTOFMEMORY)
		CASE_WINDOWS_ERROR(DXGI_ERROR_INVALID_CALL)
		CASE_WINDOWS_ERROR(E_NOINTERFACE)
		CASE_WINDOWS_ERROR(DXGI_ERROR_DEVICE_REMOVED)
	default:
		res = std::format("Code: {:x}", code);
	}
#undef CASE_WINDOWS_ERROR
	return res;
}

#define VERIFY(x) { HRESULT hres = x; if (FAILED(hres)) { check_failed(__FILE__, __LINE__, #x, get_error_string(hres)); DEBUG_BREAK();} }

template<class T>
using WinComPtr = Microsoft::WRL::ComPtr<T>;

inline void d3d_set_debug_name(IDXGIObject& object, const std::string& debug_name)
{
	object.SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(debug_name.size()), debug_name.c_str());
}
	
inline void d3d_set_debug_name(ID3D12Object& object, const std::wstring& debug_name)
{
	object.SetName(debug_name.c_str());
}

namespace t
{
	template<typename t_gapi_dynamic_impl, typename t_gapi_interface>
	std::shared_ptr<t_gapi_dynamic_impl> gapi_cast(const std::shared_ptr<t_gapi_interface>& inst)
	{
		static_assert(std::is_base_of_v<t_gapi_interface, t_gapi_dynamic_impl> == true, "Invalid inheritance for `gapi_cast(...)`!");
#if GAPI_USE_DYNAMIC_CAST
		return std::dynamic_pointer_cast<t_gapi_dynamic_impl>(inst);
#else  // GAPI_USE_DYNAMIC_CAST
		return std::reinterpret_pointer_cast<t_gapi_dynamic_impl>(inst);
#endif // GAPI_USE_DYNAMIC_CAST
	}

	template<typename t_gapi_dynamic_impl, typename t_gapi_interface>
	static t_gapi_dynamic_impl& gapi_pin(const std::shared_ptr<t_gapi_interface>& inst)
	{
		static_assert(std::is_base_of_v<t_gapi_interface, t_gapi_dynamic_impl> == true, "Invalid inheritance for `gapi_pin(...)`!");
		CHECK(inst != nullptr);
		//
#if GAPI_USE_DYNAMIC_CAST
		return dynamic_cast<t_gapi_dynamic_impl&>(*inst);
#else  // GAPI_USE_DYNAMIC_CAST
		return reinterpret_cast<t_gapi_dynamic_impl&>(*inst);
#endif // GAPI_USE_DYNAMIC_CAST
	}
}