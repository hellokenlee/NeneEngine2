/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

class device
{
public:
	virtual void init() = 0;
	virtual void shutdown() = 0;

public:
	static device* get();
	static void set(device* in_device);

private:
	static device* m_device;
};