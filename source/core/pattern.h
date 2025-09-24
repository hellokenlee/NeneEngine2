/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/**
 *	Usage:
 *	```
 *	class my_class: noncopyable
 *	{
 *	public:
 *		
 *	};
 *	```
 */
class NENE_API noncopyable 
{
public:
	//
	noncopyable() = default;
	// rule of five
	virtual ~noncopyable() = default;
	noncopyable(const noncopyable&) = delete;
	noncopyable(noncopyable&&) = default;
	noncopyable& operator=(const noncopyable&) = delete;
	noncopyable& operator=(noncopyable&&) = default;
};


