/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "meta.h"
#include "serializable.h"


namespace n
{
	class NENE_API object: public i::serializable
	{
	public:
		object()
			: m_data0(456)
			, m_data1(234)
		{}
		~object() override = default;

		void serialize(i::archive& ar) override;

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
