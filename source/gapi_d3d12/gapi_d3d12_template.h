/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

#define GAPI_D3D12_USE_DYNAMIC_CAST 0

namespace t
{
	template<class gapi_d3d12_dynamic_type, class gapi_interface_type, class d3d12_implement_type>
	class dynamic: public gapi_interface_type, public d3d12_implement_type
	{
	public:
		typedef dynamic super;

		using d3d12_implement_type::d3d12_implement_type;

		static shared_ptr<gapi_d3d12_dynamic_type> cast(shared_ptr<gapi_interface_type> inst)
		{
#if GAPI_D3D12_USE_DYNAMIC_CAST
			return dynamic_pointer_cast<gapi_d3d12_dynamic_type>(inst);
#else
			return reinterpret_pointer_cast<gapi_d3d12_dynamic_type>(inst);
#endif
		}
	};

	template<class gapi_d3d12_dynamic_type, class gapi_interface_type>
	class dynamic_custom: public gapi_interface_type
	{
	public:
		static shared_ptr<gapi_d3d12_dynamic_type> cast(shared_ptr<gapi_interface_type> inst)
		{
#if GAPI_D3D12_USE_DYNAMIC_CAST
			return dynamic_pointer_cast<gapi_d3d12_dynamic_type>(inst);
#else
			return reinterpret_pointer_cast<gapi_d3d12_dynamic_type>(inst);
#endif
		}
	};
}
