#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"

#include <string>
#include <unordered_map>
#include <utility>      // move

namespace frog::scripts {

class ProfilerGuard
{
    static std::unordered_map<std::string, double> times_us;

    std::string name;

public:
    ProfilerGuard(std::string name)
        : name(std::move(name))
    {
    }

    ProfilerGuard(const ProfilerGuard&) = delete;
    ProfilerGuard& operator=(const ProfilerGuard&) = delete;

    ~ProfilerGuard()
    {
        double us = 0;
        times_us[name] += us;
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
