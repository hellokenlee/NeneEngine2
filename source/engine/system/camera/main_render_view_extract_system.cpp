/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "main_render_view_extract_system.h"
#include "component/camera_component.h"
#include "core_render/render_thread.h"


namespace nene::g
{
	main_render_view_extract_system::main_render_view_extract_system(flecs::world& ecs)
		: m_render_view(std::make_shared<r::render_view>())
	{
		ecs.system("").run(
			[this](flecs::iter& it)
			{
				flecs::world ecs = it.world();
				auto main_camera_entity = ecs.target<main_camera_relation>();
				const auto& cam = main_camera_entity.get<camera_component>();
				if (main_camera_entity.is_valid())
				{
					// TODO: 置脏判断
					enqueue_render_command<"MainRenderView::Update">(
						[render_view = m_render_view, view_location = cam.m_location, view_direction = cam.m_forward, fov = cam.m_fov, ratio = cam.m_ratio, near = cam.m_near, far = cam.m_far]()
						{
							render_view->update_view_matrix(view_location, view_direction);
							render_view->set_perspective_projection_matrix(fov, ratio, near, far);
						}
					);
				}
			}
		);
	}
}
