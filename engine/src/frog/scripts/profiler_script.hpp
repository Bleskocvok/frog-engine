#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/utils/string.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>      // move

#define FROG_PROFILE_FUNC() auto guard = ::frog::scripts::ProfilerGuard()

namespace frog::scripts {

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

class ProfilerScript : public frog::script2d
{
    double accum = 0;

    std::map<std::string, ProfilerGuard::Item> times_us;
    decltype(times_us) snapshot;

    // void out(std::ostream& o) const
    // {
    //     for (const auto& val : times_us)
    //     {
    //         out_line(o, val);
    //         o << "\n";
    //     }

    //     o << "\n";
    // }

    bool emit = false;
    bool changed_announce = false;

public:
    ProfilerScript()
    { }

    void stable_update(frog::game_object2d&, frog::engine2d&) override
    {
        changed_announce = false;

        if (emit)
        {
            emit = false;

            changed_announce = true;
        }
    }

    void frame_update(frog::game_object2d&, frog::engine2d& e) override
    {
        accum += e.global->frame_time();

        for (const auto&[key, us] : ProfilerGuard::times())
            times_us[key] += us;

        ProfilerGuard::reset();

        if (accum >= 1)
        {
            accum -= 1;

            // print();
            snapshot = times_us;
            emit = true;

            times_us.clear();
        }
    }

    void out_line(std::ostream& o, const std::pair<std::string, ProfilerGuard::Item>& val) const
    {
        const auto&[key, item] = val;
        auto ms = item.sum / 1000.0;
        o
            << std::right
            << std::setw(8)
            << std::fixed
            << std::setprecision(3)
            << ms << " ms "

            << std::left
            << std::setw(18)
            << key

            << " |"
            << std::right
            << std::setw(8)
            << std::fixed
            << std::setprecision(3)
            << item.avg() / 1000.0
            << " ms avg"
        ;
    }

    // void print() const
    // {
    //     out(std::cout);
    // }

    // std::string to_string() const
    // {
    //     std::ostringstream o;
    //     out(o);
    //     return std::move(o).str();
    // }

    const auto& times()
    {
        return snapshot;
    }

    bool changed() const
    {
        return changed_announce;
    }
};

} // namespace frog::scripts
