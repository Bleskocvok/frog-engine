#pragma once

#include "frog/core/script.hpp"
#include "frog/graphics/ui_element.hpp"


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

    virtual void init(GameObject& obj) {}

    virtual void idle(GameObject& obj) {}

    virtual void hover(GameObject& obj) {}

    virtual void press(GameObject& obj) {}

    virtual void stable_update(GameObject& obj) {}
};


} // namespace frog
