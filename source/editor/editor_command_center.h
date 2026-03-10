/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "editor_command/editor_command.h"


namespace nene
{
	class NENE_API editor_command_center
	{
	public:
		//
		static editor_command_center& get();
		
		//
		void invoke(editor_command& cmd);
		
	protected:
		editor_command_center() = default;
	};
}
