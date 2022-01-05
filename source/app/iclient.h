/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

class iclient
{
public:
	iclient() = default;
	virtual ~iclient() = default;

public:
	virtual bool should_exit() = 0;

	virtual void poll_messages() = 0;
};
