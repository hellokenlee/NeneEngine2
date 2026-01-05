/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_resource.h"
#include "gapi/gapi_resource.h"

namespace r
{
	template<typename t_shader_struct>
	class render_constant_buffer : public render_resource
	{
	public:
		render_constant_buffer()
		{
			memset(m_data, 0, sizeof(m_data));
		}

		void update()
		{
			m_gapi_buffer->map(
				[this](void* mapped)
				{
					memcpy(mapped, &m_data, sizeof(m_data));
				}
			);
		}

		const std::shared_ptr<i::gapi_buffer>& get_constant_buffer()
		{
			return m_gapi_buffer;
		}
		
		t_shader_struct m_data;
		
	protected:
		std::shared_ptr<i::gapi_buffer> m_gapi_buffer;
	};
}
