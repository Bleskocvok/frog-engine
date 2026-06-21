#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/utils/profiler_guard.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>      // move

namespace frog::scripts {

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
