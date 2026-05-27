/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API pix_capture_command : editor_command
	{
		void execute() override;
	};
}