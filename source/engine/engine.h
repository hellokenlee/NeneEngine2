/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <core/windll.h>

namespace nene::g
{
	class camera_component;		
}

namespace nene
{
	class NENE_API engine
	{
	public:
		engine();
		virtual ~engine() = default;

		virtual void update(std::chrono::milliseconds delta) {}

		const g::camera_component& get_camera() const { return *m_camera; }

	protected:
		std::shared_ptr<g::camera_component> m_camera;
	};
}
