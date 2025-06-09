/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace i
{
	class client
	{
	public:
		client() = default;
		virtual ~client() = default;
		
		virtual bool should_exit() = 0;

		virtual void poll_message() = 0;

		virtual void* get_window() = 0;
	};
}
