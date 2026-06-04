#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>      // move

namespace frog::scripts {

class ProfilerGuard
{
    static std::unordered_map<std::string, std::uint64_t> times_us;

    std::string name;

using Timer = frog::os::timer;

    Timer timer;

public:
    ProfilerGuard(std::string name)
        : name(std::move(name))
    {
        timer.reset();
    }

    ProfilerGuard(const ProfilerGuard&) = delete;
    ProfilerGuard& operator=(const ProfilerGuard&) = delete;

    ~ProfilerGuard()
    {
        times_us[name] += timer.duration_us();
    }

    static void reset()
    {

    }

    static const auto& times()
    {
        return times_us;
    }
};

class ProfilerScript : public frog::script2d
{
public:
    ProfilerScript()
    { }

    void init(frog::game_object2d& obj, frog::engine2d&) override
    {

    }

    void stable_update(frog::game_object2d& obj, frog::engine2d&) override
    {

    }

    static const auto& times()
    {
        return ProfilerGuard::times();
    }
};

} // namespace frog::scripts
