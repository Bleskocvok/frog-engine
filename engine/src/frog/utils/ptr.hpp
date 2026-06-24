#pragma once

#include "frog/utils/exception.hpp"

#include <memory>	// std::unique_ptr
#include <utility>	// std::forward, std::move


namespace frog
{


template<typename T>
using ptr = std::unique_ptr<T>;

// template<typename T>
// using weak = std::weak_ptr<T>;

// TODO: std::weak_ptr, but that requires change in object creation.
template<typename T>
using weak = T*;

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
shared<T> mk_shared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct safe_ptr
{
    safe_ptr() = default;

    safe_ptr(T* ptr)
        : ptr(ptr)
    { }

    T& operator*()
    {
        if (ptr == nullptr)
            throw frog::error("safe_ptr: dereferencing nullptr");

        return *ptr;
    }

    const T& operator*() const
    {
        if (ptr == nullptr)
            throw frog::error("safe_ptr: dereferencing const nullptr");

        return *ptr;
    }

    safe_ptr& operator=(T* ptr)
    {
        this->ptr = ptr;
        return *this;
    }

    T* operator->()
    {
        return &operator*();
    }

    const T* operator->() const
    {
        return &operator*();
    }

    operator bool() const noexcept
    {
        return ptr != nullptr;
    }

private:
    T* ptr = nullptr;
};

} // namespace frog
