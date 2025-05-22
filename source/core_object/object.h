/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "meta.h"
#include "core/core.h"
#include "serializable.h"


namespace n
{
	class NENE_API object: public serializable
	{
	public:
		object(int data)
			: m_data0(data)
			, m_data1(234)
		{}
		virtual ~object() = default;

		void func0() const;

		void func1(int a) const;

		static void test();

	public:
		int m_data0;
		int m_data1;

		static int s_data2;
		static int s_data3;
	};
}
