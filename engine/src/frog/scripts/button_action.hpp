#pragma once

#include "frog/core/script.hpp"

namespace frog {

template<typename GameObject>
struct button_action_base;

using button_action   = button_action_base<game_object>;
using button_action2d = button_action_base<game_object2d>;

template<typename GameObject>
struct button_action_base
{
    virtual ~button_action_base() = default;

    virtual void action(GameObject&) {}
    virtual void action() {}

    virtual void frame_holding(GameObject&) {}
    virtual void frame_holding() {}

    virtual void stable_holding(GameObject&) {}
    virtual void stable_holding() {}
};


}       // namespace frog

