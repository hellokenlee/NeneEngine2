/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_constant_buffer.h"
#include "shader/cppshared/view_uniform_buffer.h"

namespace r
{
	class NENE_API render_view : public render_constant_buffer<SViewInfo>
	{
	public:
		void update_view_matrix(const float3& view_location, const rotator& view_rotator);
	};
}