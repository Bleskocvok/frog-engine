#pragma once

#include "frog/scripts/timer_script.hpp"

#include <functional>
#include <optional>
#include <utility>

namespace frog::scripts {

template<typename Script>
class RepeatBase;

using Repeat2d = RepeatBase<frog::timer_script2d>;
using Repeat   = RepeatBase<frog::timer_script>;

template<typename Script>
class RepeatBase : public Script
{
    std::function<void()> func;

    double seconds = 0;
    unsigned count = 0;

public:
    RepeatBase(double secs_,
            unsigned count_,
            std::function<void()> func_,
            std::optional<double> first_delay_ = std::nullopt)
        : Script(0, Script::before)
        , func(std::move(func_))
        , seconds(secs_)
        , count(count_)
    {
        Script::reset_accum();
        Script::set_period(first_delay_.value_or(0));
    }

    void stable_update(typename Script::GameObject& o, typename Script::Engine& e) override
    {
        Script::stable_update(o, e);

        if (count <= 0)
        {
            o.remove_script(this);
            return;
        }

        if (this->activations())
        {
            if (func)
                func();

            count--;

            // TODO: reset_accum stinks. Replace with version that doesn't
            // drift.
            Script::reset_accum();
            Script::set_period(seconds);
        }
    }
};

} // namespace frog::scripts
