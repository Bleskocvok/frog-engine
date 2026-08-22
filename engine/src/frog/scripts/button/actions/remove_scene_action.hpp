#pragma once

#include "frog/scripts/button/button_action.hpp"

#include <string>

namespace frog {

template<typename Base>
struct remove_scene_action_base;

using remove_scene_action   = remove_scene_action_base<button_action>;
using remove_scene_action2d = remove_scene_action_base<button_action2d>;

template<typename Base>
struct remove_scene_action_base : Base
{
    std::string name;

    explicit remove_scene_action_base(std::string name)
        : name(std::move(name)) {}

    void action(typename Base::GameObject&, typename Base::Engine& engine) override
    {
        engine.scenes->remove(name);
    }
};

} // namespace frog
