/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once
#pragma warning(disable : 4250)

#include "core/core.h"
#include "gapi/gapi.h"

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3dx12.h"



#define VERIFY(x) { HRESULT hres = x; if (FAILED(hres)) { verify_impl(hres, TXT(#x), TXT(__FILE__), __LINE__); }}

template<class T>
using WinComPtr = Microsoft::WRL::ComPtr<T>;

DECLARE_LOG_CATEGORY(d3d12);

static std::string get_error_string(const HRESULT code)
{
	//
	std::string res;
	//
	#define D3DERR(x) case x: res = #x; break;
	//
	switch (code)
	{
		D3DERR(S_OK);
		D3DERR(D3D11_ERROR_FILE_NOT_FOUND)
		D3DERR(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
		D3DERR(E_FAIL)
		D3DERR(E_INVALIDARG)
		D3DERR(E_OUTOFMEMORY)
		D3DERR(DXGI_ERROR_INVALID_CALL)
		D3DERR(E_NOINTERFACE)
		D3DERR(DXGI_ERROR_DEVICE_REMOVED)
	default:
		res = std::format("Code: {}", static_cast<int32>(code));
	}

	return res;
}

EXTERN_LOG_CATEGORY(d3d12)

void inline verify_impl(const HRESULT hres, const wchar_t* code, const wchar_t* filename, uint32 line)
{
	const std::string error = get_error_string(hres);
	LOG(d3d12, fatal, "Function call failed!\n    Code at %ls:%u:\n        `%ls`\n    Error:\n        `%s`", filename, line, code, error.c_str());
	DEBUG_BREAK();
}

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
	
	/*
	 * Helper template for down casting within implementations
	 * 
	 * Example:
	 *		class gapi_d3d12_cmd_context : public t::dynamic<gapi_d3d12_cmd_list, gapi_cmd_list, void>
	 *		{
	 *		protected:
	 *			shared_ptr<d3d12_cmd_list> m_cmd_list;
	 *			shared_ptr<d3d12_cmd_allocator> m_cmd_allocator;
	 *		};
	 *		
	 * This implement gapi <cmd_context> interface with d3d12.
	 * 
	 */
	template<class t_gapi_dynamic_impl, class t_gapi_interface>
	class impl: virtual public t_gapi_interface
	{
	public:
		typedef impl super;

		using t_gapi_interface::t_gapi_interface;

		static std::shared_ptr<t_gapi_dynamic_impl> cast(const std::shared_ptr<t_gapi_interface>& inst)
		{
			//
			static_assert(std::is_abstract<t_gapi_interface>::value == true, "Template `t::impl` only support for abstract type!");
			//
#if GAPI_USE_DYNAMIC_CAST
			return std::dynamic_pointer_cast<t_gapi_dynamic_impl>(inst);
#else  // GAPI_USE_DYNAMIC_CAST
			return std::reinterpret_pointer_cast<t_gapi_dynamic_impl>(inst);
#endif // GAPI_USE_DYNAMIC_CAST
		}
	};

	/**
	 * The second version of `t::impl<>` with a extra implement class parent.  
	 */
	template<class t_gapi_dynamic_impl, class t_gapi_dynamic_impl_parent, class t_gapi_interface>
	class poly_impl: public t_gapi_dynamic_impl_parent, virtual public t_gapi_interface
	{
	public:
		typedef poly_impl super;

		using t_gapi_interface::t_gapi_interface;
		using t_gapi_dynamic_impl_parent::t_gapi_dynamic_impl_parent;

		static std::shared_ptr<t_gapi_dynamic_impl> cast(const std::shared_ptr<t_gapi_interface>& inst)
		{
			//
			static_assert(std::is_abstract_v<t_gapi_interface> == true, "Template `t::impl` only support for abstract type!");
			//
#if GAPI_USE_DYNAMIC_CAST
			return std::dynamic_pointer_cast<t_gapi_dynamic_impl>(inst);
#else  //  GAPI_USE_DYNAMIC_CAST
			return std::reinterpret_pointer_cast<t_gapi_dynamic_impl>(inst);
#endif //  GAPI_USE_DYNAMIC_CAST
		}

		static t_gapi_dynamic_impl& pin(const std::shared_ptr<t_gapi_interface>& inst)
		{
			static_assert(std::is_abstract_v<t_gapi_interface> == true, "Template `t::impl` only support for abstract type!");
			CHECK(inst != nullptr);
			//
#if GAPI_USE_DYNAMIC_CAST
			return dynamic_cast<t_gapi_dynamic_impl&>(*inst);
#else  // GAPI_USE_DYNAMIC_CAST
			return reinterpret_cast<t_gapi_dynamic_impl&>(*inst);
#endif // GAPI_USE_DYNAMIC_CAST
		}
	};
}