/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_command_center.h"


namespace nene
{
	editor_command_center& editor_command_center::get()
	{
		static editor_command_center instance;
		return instance;
	}

	void editor_command_center::invoke(editor_command& cmd)
	{
		cmd.execute();
	}
}
