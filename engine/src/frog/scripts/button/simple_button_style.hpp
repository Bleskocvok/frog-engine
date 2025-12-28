#pragma once

#include "frog/core/script.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/scripts/button/button_style.hpp"
#include "frog/utils/ptr.hpp"

namespace frog
{

template<typename GameObject>
struct simple_button_style_base;

using simple_button_style   = simple_button_style_base<game_object>;
using simple_button_style2d = simple_button_style_base<game_object2d>;

template<typename GameObject>
struct simple_button_style_base : button_style_base<GameObject>
{
    frog::gx::ui_element* ui = nullptr;

    simple_button_style_base(frog::gx::ui_element* ui)
        : ui(ui) {}

    void init(GameObject& obj) override
    {
        if (ui)
            return;

        if (obj.elements().empty())
            ui = obj.add_element(frog::mk_ptr<gx::ui_element>());
        else
            ui = obj.elements().front().get();
    }

    void idle(GameObject&) override { idle(*ui); }

    void hover(GameObject&) override { hover(*ui); }

    void press(GameObject&) override { press(*ui); }

    virtual void idle(gx::ui_element&) {}

    virtual void hover(gx::ui_element&) {}

    virtual void press(gx::ui_element&) {}
};


} // namespace frog
