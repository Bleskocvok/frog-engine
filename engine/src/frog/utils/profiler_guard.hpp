#pragma once

#include "frog/utils/string.hpp"
#include "frog/os/timer.hpp"

#include <cstdint>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>      // move

#define FROG_PROFILE_FUNC() auto guard = ::frog::ProfilerGuard()

namespace frog {

class ProfilerGuard
{
public:
    struct Item
    {
        std::uint64_t sum = 0;
        unsigned count = 0;

        Item() = default;

        double avg() const
        {
            if (count == 0)
                return 0;

            return double(sum) / count;
        }

        void put(std::uint64_t t)
        {
            sum += t;
            count++;
        }

        friend Item& operator+=(Item& a, const Item& b)
        {
            a.sum += b.sum;
            a.count += b.count;
            return a;
        }
    };

    ProfilerGuard(std::string name)
        : name(std::move(name))
    {
        timer.reset();
    }

    ProfilerGuard(const std::source_location& location = std::source_location::current())
        : name( strip( location.function_name() ) )
    {
        timer.reset();
    }

    ProfilerGuard(const ProfilerGuard&) = delete;
    ProfilerGuard& operator=(const ProfilerGuard&) = delete;

    ~ProfilerGuard()
    {
        times_us[name].put( timer.duration_us() );
    }

    static std::string strip(const char* str)
    {
        auto ooga = frog::between(std::string_view(str), ' ', '(');
        auto last = ooga.find_last_of(':');
        auto sub = ooga.substr(last);
        if (last != std::string_view::npos)
            sub.remove_prefix(1);
        return std::string( sub );

        // return std::string( frog::between(std::string_view(str), ' ', '(') );
    }

    static void reset()
    {
        times_us.clear();
    }

    static const auto& times()
    {
        return times_us;
    }

private:
    static inline std::unordered_map<std::string, Item> times_us;

    std::string name;

    using Timer = frog::os::timer;

    Timer timer;
};

} // namespace frog
