/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"
#include "scene_component/camera_component.h"
#include "input_manager.h"

nene::engine::engine()
{
	m_camera = std::make_shared<g::camera_component>();
	input_manager::get().add_listener(*m_camera);
	
	m_camera->on_add_to_scene();
}
