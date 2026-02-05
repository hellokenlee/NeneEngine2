/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "stdout_log_handler.h"
#include "debug.h"

namespace nene
{
	void stdout_log_handler::emit(const std::string& log_message)
	{
		printf("%s\n", log_message.c_str());
		ENSURE(fflush(stdout) == 0);
	}
}
