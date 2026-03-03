/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>

namespace nene::t
{
	template <typename data_t>
	struct dual_shared_ptr
	{
		std::shared_ptr<data_t> m_ptr;
		std::shared_ptr<const data_t> m_const_ptr;
	};
}