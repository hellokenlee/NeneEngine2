/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_view.h"

namespace r
{
	render_view::render_view()
	{
		update_view_matrix(float3(0.0, 0.0, 0.0), rotator(0.0, 0.0, 0.0));
	}

	void render_view::update_view_matrix(const float3& view_location, const rotator& view_rotator)
	{
		//
		m_data.ViewOrigin = view_location;
		// quick way to: M_{rotation_world_to_camera} = M_{rotation_camera_to_world}^-1
		matrix view_rotation_matrix = matrix(view_rotator).transpose();
		// quick way to: M_{translation_world_to_camera} = M_{translation_camera_to_world}^-1
		matrix view_translation_matrix(-view_location);
		// translate first, then rotate the camera
		m_data.ViewMatrix = view_translation_matrix * view_rotation_matrix;
	}
}
