/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "builtin_render_resource.h"

namespace nene::r
{
	[[maybe_unused]] static auto& g_system_vertex_buffers_auto_register = builtin_static_meshes::get();

	const builtin_static_meshes& builtin_static_meshes::get()
	{
		static builtin_static_meshes instance;
		return instance;
	}

	void builtin_static_meshes::initialize(gapi_cmd_context& cmd_context)
	{
		// nene engine always use CCW as front face
		{
			std::vector<float3> positions = {
				// +X
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{ 0.5f,  0.5f, -0.5f},
				// -X
				{-0.5f, -0.5f,  0.5f},
				{-0.5f, -0.5f, -0.5f},
				{-0.5f,  0.5f, -0.5f},
				{-0.5f,  0.5f,  0.5f},
				// +Y
				{-0.5f,  0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{-0.5f,  0.5f,  0.5f},
				// -Y
				{-0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{-0.5f, -0.5f, -0.5f},
				// +Z
				{ 0.5f, -0.5f,  0.5f},
				{-0.5f, -0.5f,  0.5f},
				{-0.5f,  0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
				// -Z
				{-0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{-0.5f,  0.5f, -0.5f},
			};
			std::vector<float3> normals = {
				// +X
				{ 1, 0, 0},
				{ 1, 0, 0},
				{ 1, 0, 0},
				{ 1, 0, 0},
				// -X
				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},
				// +Y
				{0, 1, 0}, 
				{0, 1, 0}, 
				{0, 1, 0}, 
				{0, 1, 0},
				// -Y
				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
				// +Z
				{0, 0, 1}, 
				{0, 0, 1}, 
				{0, 0, 1}, 
				{0, 0, 1},
				// -Z
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},
			};
			std::vector<float2> uvs = {
				// +X
				{0, 1},
				{1, 1},
				{1, 0},
				{0, 0},
				// -X
				{0, 1},
				{1, 1},
				{1, 0},
				{0, 0},
				// +Y
				{0, 1},
				{1, 1},
				{1, 0},
				{0, 0},
				// -Y
				{0, 1},
				{1, 1},
				{1, 0},
				{0, 0},
				// +Z
				{1, 1},
				{0, 1},
				{0, 0},
				{1, 0},
				// -Z
				{1, 1},
				{0, 1},
				{0, 0},
				{1, 0},
			};
			std::vector<uint32_t> indices =
			{
				0,  1,  2,   2,  3,  0,		// +X
				4,  5,  6,   6,  7,  4,		// -X
				8,  11, 10, 10,  9,  8,		// +Y
				12, 15, 14, 14, 13, 12,		// -Y
				16, 17, 18, 18, 19, 16,		// +Z
				20, 21, 22, 22, 23, 20,		// -Z
			};
			m_cube = std::make_shared<static_mesh>(indices, positions, normals, uvs);
		}
	}
}
