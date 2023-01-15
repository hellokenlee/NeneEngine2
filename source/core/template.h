/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


class noncopyable 
{
public:
	noncopyable() = default;
	virtual ~noncopyable() = default;

	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable) = delete;
};

