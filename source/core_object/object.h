/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "meta.h"
#include "core/core.h"
#include "serializable.h"


namespace n
{
	class NENE_API object: public i::serializable
	{
	public:
		object(int data)
			: data0(data)
		{}
		virtual ~object() = default;

		void func0();

		void func1(int a);

		static void test();

	private:
		int data0;
	};
}

void pybind11_init_object(::pybind11::module_ & m);

