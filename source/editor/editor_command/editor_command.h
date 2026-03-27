/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


namespace nene
{
	struct NENE_API editor_command
	{
		virtual ~editor_command() = default;
		//
		virtual void execute() {}
		//
		virtual void undo() {}
	};
}