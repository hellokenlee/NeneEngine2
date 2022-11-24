/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_shader.h"
#include "core/file_helper.h"

gapi_shader_initializer::gapi_shader_initializer(const string& filepath, const string& entry)
{
	m_shader_file = file_helper::wstring_to_sstring(filepath);
	m_shader_entry = file_helper::wstring_to_sstring(entry);
	m_shader_source = file_helper::load_file_to_sstring(filepath);
}

const sstring& gapi_shader_initializer::get_shader_file() const 
{
	return m_shader_file; 
}

const sstring& gapi_shader_initializer::get_shader_source() const 
{
	return m_shader_source; 
}

const sstring& gapi_shader_initializer::get_shader_entry() const 
{
	return m_shader_entry; 
}
