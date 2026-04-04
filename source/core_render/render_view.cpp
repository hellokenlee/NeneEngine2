/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_view.h"

namespace nene::r
{
	render_view::render_view()
	{
		update_view_matrix(float3::zero(), float3::forward_vector());
	}

	void render_view::update_view_matrix(const float3& view_location, const float3& view_direction)
	{
		//
		m_data.ViewOrigin = view_location;
		/*
		// quick way to: M_{rotation_world_to_camera} = M_{rotation_camera_in_world}^-1
		matrix view_rotation_matrix = matrix::make_rotation_matrix(view_rotator).transpose();
		// quick way to: M_{translation_world_to_camera} = M_{translation_camera_in_world}^-1
		matrix view_translation_matrix = matrix::make_translation_matrix(-view_location);
		// translate first, then rotate the camera
		m_data.ViewMatrix = view_translation_matrix * view_rotation_matrix;
		*/
		m_data.ViewMatrix = matrix::make_view_matrix(view_location, view_direction);
		mark_constant_buffer_dirty();
	}

	void render_view::set_perspective_projection_matrix(float fov_angle, float ratio, float near, float far)
	{
		m_data.ProjectionMatrix = matrix::make_perspective_projection_matrix(fov_angle, ratio, near, far);
		
		mark_constant_buffer_dirty();
	}

	void render_view::set_orthographic_projection_matrix(float width, float height, float near, float far)
	{
		m_data.ProjectionMatrix = matrix::make_orthographic_projection_matrix(width, height, near, far);
		
		mark_constant_buffer_dirty();
	}
}
