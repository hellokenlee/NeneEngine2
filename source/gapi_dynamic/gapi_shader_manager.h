/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi.h"
#include "gapi/gapi_shader.h"


class NENE_API gapi_shader_manager
{
public:
	static gapi_shader_manager& get();
	
	const std::shared_ptr<i::gapi_shader>& find_or_create_shader(const gapi_shader_type& stage, const std::string& source_filepath, const std::string& entry_name);

private:
	gapi_shader_manager() = default;

	
protected:
	std::unordered_map<uint64, std::shared_ptr<i::gapi_shader>> m_shader_map;
};