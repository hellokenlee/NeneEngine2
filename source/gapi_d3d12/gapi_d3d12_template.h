/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

#define GAPI_D3D12_USE_DYNAMIC_CAST 1

namespace t
{
	/*
	 * Dynamic Base Class Template With Given Implementation
	 * Usage:
	 *		class gapi_d3d12_vertex_shader : public t::dynamic<gapi_d3d12_vertex_shader, gapi_vertex_shader, d3d12_vertex_shader>
	 *		{};
	 * This type of GAPI interface has corresponding implementation in d3d12.
	 */
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

	/*
	 * Dynamic Base Class Template With Custom Implementation
	 * Usage:
	 *		class gapi_d3d12_cmd_list : public t::dynamic<gapi_d3d12_cmd_list, gapi_cmd_list, void>
	 *		{
	 *		protected:
	 *			shared_ptr<d3d12_cmd_list> m_cmd_list;
	 *		};
	 * Do not have corresponding implementation in d3d12.
	 * Usually own multiple instance of d3d12 implementation to achieve.
	 */
	template<class gapi_d3d12_dynamic_type, class gapi_interface_type>
	class dynamic<gapi_d3d12_dynamic_type, gapi_interface_type, void>: public gapi_interface_type
	{
	public:

		typedef dynamic super;

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
