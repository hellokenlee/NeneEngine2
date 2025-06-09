/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

extern t::console_var<bool> cvar_render_thread_enabled;

NENE_API bool is_render_thread_enabled();

NENE_API bool is_in_resource_thread();

NENE_API bool is_in_render_threads();

NENE_API bool is_in_main_thread();

template<t::string_literal command_name, typename t_lambda>
void enqueue_render_command(t_lambda&& lambda)
{
	if (is_render_thread_enabled())
	{
		NOT_IMPLEMENTED();
	}
	else
	{
		lambda();
	}
}
