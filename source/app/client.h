/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace i
{
class client
{
public:
	client() = default;
	virtual ~client() = default;

public:
	virtual bool should_exit() = 0;

	virtual void poll_messages() = 0;

	virtual void* get_window() = 0;
};
}
