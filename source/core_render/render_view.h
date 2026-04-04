/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_constant_buffer.h"
#include "shader/cppshared/view_info.h"


namespace nene::r
{
	class NENE_API render_view : public render_constant_buffer<SViewInfo>
	{
	public:
		render_view();
		void update_view_matrix(const float3& view_location, const float3& view_direction);
		void set_perspective_projection_matrix(float fov_angle, float ratio, float near, float far);
		void set_orthographic_projection_matrix(float width, float height, float near, float far);
	};
}