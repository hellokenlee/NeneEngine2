/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_shader.h"


class gapi_shader_manager
{
public:
	std::shared_ptr<i::gapi_shader> find_or_create_shader();
	
public:
	static std::shared_ptr<gapi_shader_manager> get();

protected:
	std::unordered_map<uint64, std::shared_ptr<i::gapi_shader>> m_shader_map;
};