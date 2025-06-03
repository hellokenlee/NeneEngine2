/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

extern t::console_var<bool> cvar_render_thread_enabled;

bool is_render_thread_enabled();

template<size_t N, t::string_literal<N> command_name, typename t_lambda>
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

bool is_in_resource_thread()
{
	
}

bool is_in_render_threads()
{
	
}

bool is_in_main_thread()
{
	
}

