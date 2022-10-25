/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <vector>
#include <queue>
#include <array>
#include <list>

namespace t
{
template<class T>
using dynamic_array = std::vector<T>;

template<class T, size_t S>
using static_array = std::array<T, S>;

template<class T>
using queue = std::queue<T>; 
}
