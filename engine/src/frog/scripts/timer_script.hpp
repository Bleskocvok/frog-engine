#pragma once

#include "frog/core/script.hpp"

namespace frog {

template<typename Script>
class timer_script_base;

using timer_script2d = timer_script_base<frog::script2d>;
using timer_script   = timer_script_base<frog::script>;

template<typename Script>
class timer_script_base : public Script
{
public:
    enum accum_policy { before, after };
private:
    float accum_ = 0;
    float period_ = 0;
    int activated = 0;
    accum_policy policy;

public:
    timer_script_base(float period_, accum_policy policy = before)
        : period_(period_), policy(policy) {}

    timer_script_base& set_period(float t)
    {
        period_ = t;
        return *this;
    }

    timer_script_base& reset_accum()
    {
        accum_ = 0;
        return *this;
    }

    int activations() const
    {
        return activated;
    }

    float accum() const { return accum_; }

    float period() const { return period_; }

    float remaining() const { return period() - accum(); }

    void init(typename Script::GameObject& obj, typename Script::Engine&) override
    {
        using namespace frog;
    }

    // TODO: Consider situations where updating here would be preferable.
    // Perhaps more policy settings?
    // void frame_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    // {
    //     using namespace frog;
    // }

    void stable_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    {
        if (policy == before)
            accum_ += engine.global->stable_frame_time();

        activated = 0;
        // TODO: Make this non-exponential, lol.
        while (accum_ >= period_)
        {
            accum_ -= period_;
            ++activated;
        }

        if (policy == after)
            accum_ += engine.global->stable_frame_time();
    }
};


}      // namespace frog
