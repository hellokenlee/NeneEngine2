#include "gapi_shader.h"
#include "core/file_helper.h"

gapi_shader_initializer::gapi_shader_initializer(const string& filepath, const string& entry)
{
	m_shader_file = file_helper::wstring_to_sstring(filepath);
	m_shader_entry = file_helper::wstring_to_sstring(entry);
	m_shader_source = file_helper::load_file<sstring>(filepath);
}
