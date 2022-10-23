/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "serializable.h"


namespace n
{
class NENE_API object: public i::serialzable
{
public:
	object(int data)
		: data0(data)
	{}

	void func0();

	void func1(int a);

	static void test();

private:
	int data0;
};
}

