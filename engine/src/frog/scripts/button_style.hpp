#pragma once

#include "frog/graphics/ui_element.hpp"


namespace frog
{


struct button_style
{
    virtual ~button_style() = default;

    virtual void idle(gx::ui_element&) = 0;

    virtual void hover(gx::ui_element&) = 0;

    virtual void press(gx::ui_element&) = 0;
};


} // namespace frog
