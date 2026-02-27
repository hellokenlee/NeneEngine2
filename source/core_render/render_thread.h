/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace nene
{
	/** check if render threads are enabled */
	NENE_API bool is_render_thread_enabled();

	/** check if current stack in render thread */
	NENE_API bool is_in_render_threads();

	/** check if current stack in game thread */
	NENE_API bool is_in_game_thread();

	/** enqueue a lambda function that will be executed in render thread */
	template<t::string_literal command_name, typename t_lambda>
	NENE_API void enqueue_render_command(t_lambda&& lambda)
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
	
	NENE_API inline void flush_render_commands() {}
}