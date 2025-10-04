#pragma once

#include "frog/core/script.hpp"
#include "frog/scripts/timer_script.hpp"

#include <functional>
#include <utility>

namespace frog {

template<typename Script>
class event_base;

using event2d = event_base<frog::timer_script2d>;
using event   = event_base<frog::timer_script>;

template<typename Script>
class event_base : public Script
{
    std::function<void()> func_;
public:
    event_base() : Script(0, Script::before)
    { }

    void do_after(float secs, std::function<void()> func)
    {
        this->reset_accum();
        this->set_period(secs);
        func_ = std::move(func);
    }

    void stable_update(typename Script::GameObject& o, typename Script::Engine& e) override
    {
        Script::stable_update(o, e);

        if (this->activations())
        {
            if (func_)
            {
                func_();
                func_ = nullptr;
            }
            this->reset_accum();
        }
    }
};

} // namespace frog
