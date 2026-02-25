/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "scene_component/scene_component.h"
#include <vector>

namespace nene::g
{
	struct scene_entity
	{
		std::vector<scene_component> m_components;
	};
}