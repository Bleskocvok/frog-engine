#pragma once

#include <chrono>
#include <thread>


namespace frog::os {


using clock = std::chrono::steady_clock;

using s = std::chrono::seconds;
using ms = std::chrono::milliseconds;
using us = std::chrono::microseconds;


class timer
{
    clock::time_point point;

public:
    timer()
        : point(clock::now()) {}

    void reset()
    {
        point = clock::now();
    }

    template<typename T>
    [[nodiscard]]
    unsigned duration() const
    {
        return std::chrono::duration_cast<T>(clock::now() - point).count();
    }

    template<typename T>
    [[nodiscard]]
    unsigned reset_duration()
    {
        auto pt = point;
        point = clock::now();
        return std::chrono::duration_cast<T>(point - pt).count();
    }

    template<typename T>
    void sleep(unsigned value)
    {
        auto time = T(value);
        std::this_thread::sleep_for(time);
    }

    [[nodiscard]] unsigned duration_us() const { return duration<os::us>(); }

    [[nodiscard]] unsigned reset_duration_us() { return reset_duration<os::us>(); }
};


} // namespace frog
