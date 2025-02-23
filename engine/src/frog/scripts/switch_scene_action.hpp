#pragma once

#include "button_action.hpp"

#include <string>

namespace frog {

template<typename Base>
struct switch_scene_action_base;

using switch_scene_action   = switch_scene_action_base<button_action>;
using switch_scene_action2d = switch_scene_action_base<button_action2d>;

template<typename Base>
struct switch_scene_action_base : Base
{
    std::string name;

    explicit switch_scene_action_base(std::string name)
        : name(std::move(name)) {}

    void action(Base::GameObject&, Base::Engine& engine) override
    {
        engine.scenes->switch_scene(name);
    }

};

} // namespace frog
