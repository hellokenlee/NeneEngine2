/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>
#include <utility>

namespace t
{
    template <typename T>
    using weak_ptr = std::weak_ptr<T>;

    template <typename T>
    using shared_ptr = std::shared_ptr<T>;

    template <typename T>
    using unique_ptr = std::unique_ptr<T>;

    template <typename T>
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

    template <class T1, class T2>
    shared_ptr<T1> dynamic_pointer_cast(const shared_ptr<T2>& other) noexcept
    {
        const auto ptr = dynamic_cast<typename shared_ptr<T1>::element_type*>(other.get());

        if (ptr)
        {
            return shared_ptr<T1>(other, ptr);
        }

        return {};
    }

    template <class T1, class T2>
    shared_ptr<T1> dynamic_pointer_cast(shared_ptr<T2>&& other) noexcept {
        const auto ptr = dynamic_cast<typename shared_ptr<T1>::element_type*>(other.get());

        if (ptr)
        {
            return shared_ptr<T1>(_STD move(other), ptr);
        }

        return {};
    }
}
