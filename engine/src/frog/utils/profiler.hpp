#pragma once

#include <cstdint>
namespace frog {

template<typename Timer>
class profiler {

    Timer timer;

    std::uint64_t accum = 0;
    std::uint64_t last = 0;
    int count = 0;
    int avg_over = 0;

public:
    explicit profiler(int avg_over = 10)
        : avg_over(avg_over) {}

    auto get() const { return last; }

    void start()
    {
        timer.reset();
    }

    void end()
    {
        accum += timer.reset_duration_us();
        if (++count >= avg_over)
        {
            last = accum / count;
            count = 0;
            accum = 0;
        }
    }
};

} // namespace frog
