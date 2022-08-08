/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

class i_client
{
public:
	i_client() = default;
	virtual ~i_client() = default;

public:
	virtual bool should_exit() = 0;

	virtual void poll_messages() = 0;

	virtual void* get_window() = 0;
};
