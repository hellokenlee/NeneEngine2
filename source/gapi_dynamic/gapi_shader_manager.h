/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_shader.h"


class gapi_shader_manager
{
public:
	t::shared_ptr<i::gapi_shader> find_or_create_shader();
	
public:
	static t::shared_ptr<gapi_shader_manager> get();

protected:
	t::map<uint64, t::shared_ptr<i::gapi_shader>> m_shader_map;
};