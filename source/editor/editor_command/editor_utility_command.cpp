/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_utililty_command.h"
#include "engine/engine_loop.h"

namespace nene
{
	void pix_capture_command::execute()
	{
		engine_loop::debug_capture_gpu_frame();
	}
}
