/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "serializable.h"

namespace n
{
class object: public i::serialzable
{
public:

	void func0();

	void func1(int a);


private:
	int data0;
};
}

