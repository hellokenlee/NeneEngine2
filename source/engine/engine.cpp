/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"
#include "camera.h"
#include "input_manager.h"

nene::engine::engine()
{
	m_camera = std::make_shared<g::camera>();
	input_manager::instance().add_listener(*m_camera);
}
