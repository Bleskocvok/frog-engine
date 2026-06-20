#pragma once

#include "frog/scripts/timer_script.hpp"

#include <functional>
#include <utility>

namespace frog::scripts {

template<typename Script>
class DoOnceBase;

using DoOnce2d = DoOnceBase<frog::timer_script2d>;
using DoOnce   = DoOnceBase<frog::timer_script>;

template<typename Script>
class DoOnceBase : public Script
{
    std::function<void()> func;

public:
    DoOnceBase(double secs, std::function<void()> func)
        : Script(0, Script::before)
        , func(std::move(func))
    {
        Script::reset_accum();
        Script::set_period(secs);
    }

    void stable_update(typename Script::GameObject& o, typename Script::Engine& e) override
    {
        Script::stable_update(o, e);

        if (this->activations())
        {
            if (func)
                func();

            o.remove_script(this);
        }
    }
};

} // namespace frog::scripts
