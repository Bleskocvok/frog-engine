#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/core/script.hpp"
#include "frog/utils/profiler_guard.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>      // move

namespace frog::scripts {

class ProfilerScript : public frog::script2d
{
    double accum = 0;

    std::map<ProfilerGuard::Key, ProfilerGuard::Item> times_us;
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

    void stable_update(frog::game_object2d&, frog::engine2d&) override;

    void frame_update(frog::game_object2d&, frog::engine2d& e) override;

    using Columns = std::tuple<int, int, int>;

    void out_line(std::ostream& o,
                  const std::pair<ProfilerGuard::Key, ProfilerGuard::Item>& val,
                  Columns columns = Columns{ 18, 8, 8 }) const
    {
        auto [one, two, thr] = columns;

        const auto&[key, item] = val;
        auto ms = item.sum / 1000.0;
        o
            << std::left
            << std::setw(one)
            << key.first

            << " |"
            << std::right
            << std::setw(two)
            << std::fixed
            << std::setprecision(3)
            << ms << ""

            << " |"
            << std::right
            << std::setw(thr)
            << std::fixed
            << std::setprecision(3)
            << item.avg() / 1000.0
            << ""
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

#endif
