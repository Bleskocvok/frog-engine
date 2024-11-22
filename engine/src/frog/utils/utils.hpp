#pragma once

namespace frog {

struct non_movable
{
    non_movable() = default;

    non_movable(const non_movable&) = delete;
    non_movable(non_movable&&) = delete;
    non_movable& operator=(non_movable&) = delete;
    non_movable& operator=(non_movable&&) = delete;
    ~non_movable() = default;
};

}  // namespace frog
