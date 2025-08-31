#pragma once

#include "frog/core/script.hpp"

namespace frog {

template<typename GameObject, typename Engine>
struct button_action_base;

using button_action   = button_action_base<game_object, engine>;
using button_action2d = button_action_base<game_object2d, engine2d>;

template<typename GObject, typename EEngine>
struct button_action_base
{
    using GameObject = GObject;
    using Engine = EEngine;

    virtual ~button_action_base() = default;

    virtual void action(GameObject&, Engine&) {}
    virtual void action() {}

    virtual void frame_holding(GameObject&, Engine&) {}
    virtual void frame_holding() {}

    virtual void stable_holding(GameObject&, Engine&) {}
    virtual void stable_holding() {}
};


}       // namespace frog

