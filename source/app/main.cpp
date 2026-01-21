/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/log.h"
#include "core/log_handler.h"

int main()
{
	// 
	logger::add_handler(std::make_shared<stdout_log_handler>());
	// create client
	client runtime_client;
	if (runtime_client.get_window() == nullptr)
	{
		return -1;
	}
	// main loop
	while (!runtime_client.should_exit())
	{
		runtime_client.update();
	}
	return 0;
}