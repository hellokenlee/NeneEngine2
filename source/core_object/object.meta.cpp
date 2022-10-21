/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"

void f()
{
	
}

META
{
	n::method<decltype(f)>("f", &f);
};