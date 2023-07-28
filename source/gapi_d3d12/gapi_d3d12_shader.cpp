/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_shader.h"
#include "d3d12_shader_compiler.h"

DEFINE_LOG_CATEGORY(shader);


bool gapi_d3d12_shader::compile()
{
	d3d12_fxc_shader_compiler compiler;

	m_is_compiled = compiler.compile_shader(*this);

	if (!m_is_compiled)
	{
		SLOG(shader, warning, "Failed to get reflection data from %s::%s:", m_name.c_str(), m_function_entry.c_str());

		if (m_compiler_message != nullptr)
		{
			SLOG(shader, error, "    %s", m_compiler_message->GetBufferPointer());
		}	
	}
	else
	{
		m_is_compiled = compiler.reflect_shader(*this);

		if (!m_is_compiled)
		{
			SLOG(shader, error, "Failed to get reflection data from %s::%s:", m_name.c_str(), m_function_entry.c_str());
		}
	}

	return m_is_compiled;
}
