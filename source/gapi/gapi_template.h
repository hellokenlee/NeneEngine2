/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

#define GAPI_USE_DYNAMIC_CAST 1

namespace t
{
	/*
	 * Dynamic Base Class Template With Given Implementation
	 * Usage:
	 *		class gapi_xxx_vertex_shader : public t::dynamic<gapi_xxx_vertex_shader, gapi_vertex_shader, xxx_vertex_shader>
	 *		{};
	 * This type of GAPI interface has corresponding implementation in xxx driver.
	 */
	template<class gapi_dynamic_type, class gapi_interface_type, class dynamic_implement_type>
	class dynamic: public gapi_interface_type, public dynamic_implement_type
	{
	public:

		typedef dynamic super;

		using dynamic_implement_type::dynamic_implement_type;

		static shared_ptr<gapi_dynamic_type> cast(shared_ptr<gapi_interface_type> inst)
		{
#if GAPI_USE_DYNAMIC_CAST
			return dynamic_pointer_cast<gapi_dynamic_type>(inst);
#else
			return reinterpret_pointer_cast<gapi_dynamic_type>(inst);
#endif
		}
	};

	/*
	 * Dynamic Base Class Template With Custom Implementation
	 * Usage:
	 *		class gapi_xxx_cmd_context : public t::dynamic<gapi_xxx_cmd_list, gapi_cmd_list, void>
	 *		{
	 *		protected:
	 *			shared_ptr<xxx_cmd_list> m_cmd_list;
	 *			shared_ptr<xxx_cmd_allocator> m_cmd_allocator;
	 *		};
	 * Do not have corresponding implementation in xxx.
	 * Usually own multiple instance of xxx implementation to achieve.
	 */
	template<class gapi_dynamic_type, class gapi_interface_type>
	class dynamic<gapi_dynamic_type, gapi_interface_type, void>: public gapi_interface_type
	{
	public:

		typedef dynamic super;

		static shared_ptr<gapi_dynamic_type> cast(shared_ptr<gapi_interface_type> inst)
		{
#if GAPI_USE_DYNAMIC_CAST
			return dynamic_pointer_cast<gapi_dynamic_type>(inst);
#else
			return reinterpret_pointer_cast<gapi_dynamic_type>(inst);
#endif
		}
	};
}
