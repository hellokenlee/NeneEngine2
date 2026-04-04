/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_resource.h"
#include "gapi/gapi_resource.h"
#include "gapi_dynamic/gapi_dynamic.h"

#include <boost/type_index.hpp>


namespace nene::r
{
	template<typename t_shader_struct>
	class render_constant_buffer : public render_resource
	{
	public:
		render_constant_buffer()
		{
			//
			memset(&m_data, 0, sizeof(m_data));
			//
			auto desc = gapi_buffer_desc::create(sizeof(t_shader_struct), gapi_buffer_usage_flag::dynamic_buffer | gapi_buffer_usage_flag::constant_buffer);
			m_gapi_buffer = gapi_dynamic::get().create_buffer(desc);
			m_gapi_buffer->set_debug_name(std::format("ConstantBuffer<{}>", boost::typeindex::type_id_with_cvr<t_shader_struct>().pretty_name()));
		}

		void update_constant_buffer() const
		{
			if (m_is_dirty)
			{
				m_is_dirty = false;
				m_gapi_buffer->map(
					[this](void* mapped)
					{
						memcpy(mapped, &m_data, sizeof(m_data));
					}
				);
			}
		}
		
		void mark_constant_buffer_dirty() const
		{
			m_is_dirty = true;
		}

		const std::shared_ptr<gapi_buffer>& get_constant_buffer() const
		{
			return m_gapi_buffer;
		}
		
		t_shader_struct m_data;
		
	protected:
		// for data initialization
		mutable bool m_is_dirty : 1 = true;
		std::shared_ptr<gapi_buffer> m_gapi_buffer;
	};
}
