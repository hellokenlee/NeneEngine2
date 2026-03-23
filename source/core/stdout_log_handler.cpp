/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "stdout_log_handler.h"
#include "log.h"
#include "debug.h"
#include <cstdio>

namespace nene
{
	void stdout_log_handler::on_notify(const event& e)
	{
		if (auto le = e.cast<log_message_event>(); le != nullptr)
		{
			printf("%s\n", le->m_message.c_str());
			ENSURE(fflush(stdout) == 0);
		}
	}
}
