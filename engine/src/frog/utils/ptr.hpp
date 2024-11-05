#pragma once

#include <memory>	// std::unique_ptr
#include <utility>	// std::forward, std::move


namespace frog
{


template<typename T>
using ptr = std::unique_ptr<T>;

template<typename T>
using weak = std::weak_ptr<T>;

template<typename T>
using shared = std::shared_ptr<T>;

// std::make_unique
// frog::mk_ptr
// second is shorter
template<typename T, typename... Args>
ptr<T> mk_ptr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// std::make_shared
// frog::mk_shared
template<typename T, typename... Args>
ptr<T> mk_shared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}


} // namespace frog
