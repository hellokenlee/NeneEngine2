/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>

namespace t
{
template<class T>
using shared_ptr = std::shared_ptr<T>;

template<class T>
using unique_ptr = std::unique_ptr<T>;

template<class T>
using enable_shared_from_this = std::enable_shared_from_this<T>;

template <typename T, typename ...Args>
auto make_shared(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename ...Args>
auto make_unique(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
}
