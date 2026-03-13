/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include <vector>


namespace nene::t
{
	static std::vector<std::string_view> split(const std::string_view& str, char separator)
	{
		std::vector<std::string_view> result;
		if (str.empty())
		{
			result.emplace_back();
			return result;
		}
		//
		auto left = str.begin();
		for (auto it = left; it != str.end(); ++it)
		{
			if (*it == separator)
			{
				result.emplace_back(&*left, it - left);
				left = it + 1;
			}
		}
		if (left != str.end())
		{
			result.emplace_back(&*left, str.end() - left);
		}
		else if (str.back() == separator)
		{
			result.emplace_back();
		}
		return result;
	}
}
