/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "input_manager.h"

namespace nene
{
	input_manager& input_manager::instance()
	{
		input_manager _instance;
		return _instance;
	}
}
