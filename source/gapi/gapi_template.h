/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

#define GAPI_USE_DYNAMIC_CAST 1

namespace t
{
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
	template<class gapi_dynamic_type, class gapi_interface_type>
	class impl: public gapi_interface_type
	{
	public:
		typedef impl super;

		using gapi_interface_type::gapi_interface_type;

		static shared_ptr<gapi_dynamic_type> cast(shared_ptr<gapi_interface_type> inst)
		{
			// static_assert(is_abstract<gapi_interface_type>::value == true, "Template `t::impl` only support for abstract type!");

#if GAPI_USE_DYNAMIC_CAST
			return dynamic_pointer_cast<gapi_dynamic_type>(inst);
#else
			return reinterpret_pointer_cast<gapi_dynamic_type>(inst);
#endif
		}
	};
}
