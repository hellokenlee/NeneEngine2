/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

// >>> Engine Core Globals >>>
extern bool g_engine_requested_exit;

// <<< Engine Core Globals <<<


// >>> Multi Thread Rendering >>>

// <<< Multi Thread Rendering <<<


void inline request_engine_exit() 
{
	g_engine_requested_exit = true;
}
