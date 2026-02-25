/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_thread.h"

namespace nene
{
	t::console_var<bool> cvar_render_thread_enabled("render.render_thread_enabled", false, "if to use render thread or not");

	bool is_render_thread_enabled()
	{
		return false;
	}
}
