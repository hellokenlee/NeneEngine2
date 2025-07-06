/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_shader.h"
#include "d3d12_shader_compiler.h"

static logger shader_("shader");

static t::console_var<bool> gapi_shader_keep_sources("gapi.d3d.shader.keep_sources", true, "", console_var_flag::read_only);


bool gapi_d3d12_shader::compile()
{
	d3d12_fxc_shader_compiler compiler;

	m_is_compiled = compiler.compile_shader(*this, *m_bytecode.GetAddressOf(), *m_compiler_message.GetAddressOf());

	if (!m_is_compiled)
	{
		log(shader_, error, "Failed to compile shader ( %s::%s(...) ) with compiler errors:\n\t%s", m_name.c_str(), m_function_entry.c_str(), m_compiler_message ?  m_compiler_message->GetBufferPointer() : "");
	}
	else
	{
		m_is_compiled = compiler.reflect_shader(*this, m_shader_desc, m_shader_input_bind_descs);

		if (!m_is_compiled)
		{
			log(shader_, warning, "Failed to get reflection data from shader ( %s::%s(...) )", m_name.c_str(), m_function_entry.c_str());
		}
	}

	return m_is_compiled;
}
