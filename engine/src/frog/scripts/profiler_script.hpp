#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/animation.hpp"

#include <string>
#include <unordered_map>
#include <utility>      // move

namespace frog {

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
};

} // namespace frog
