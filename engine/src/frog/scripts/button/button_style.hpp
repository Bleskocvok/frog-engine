#pragma once

#include "frog/core/script.hpp"


namespace frog
{

template<typename GameObject>
struct button_style_base;

using button_style = button_style_base<game_object>;
using button_style2d = button_style_base<game_object2d>;

template<typename GameObject>
struct button_style_base
{
    virtual ~button_style_base() = default;

    virtual void init(GameObject&) {}

    virtual void idle(GameObject&) {}

    virtual void hover(GameObject&) {}

    virtual void press(GameObject&) {}

    virtual void stable_update(GameObject&) {}
};


} // namespace frog
