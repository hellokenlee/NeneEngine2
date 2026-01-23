/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "camera.h"
#include <chrono>

namespace n
{
	class NENE_API engine
	{
	public:
		engine() = default;
		virtual ~engine() = default;

		virtual void update(std::chrono::milliseconds delta) {}

		const camera& get_camera() const { return m_camera; }

	protected:
		camera m_camera;
	};
}
