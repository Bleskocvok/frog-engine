#pragma once

#include "frog/core/script.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/collision.hpp"
#include "frog/graphics/ui_element.hpp"

#include "button_action.hpp"
#include "button_style.hpp"

#include <utility>      // move


namespace frog {


template<typename Script>
class button_script_base;


using button_script2d = button_script_base<frog::script2d>;
using button_script   = button_script_base<frog::script>;


template<typename Script>
class button_script_base : public Script
{
    frog::gx::ui_element* ui = nullptr;
    bool down = false;

    enum state_t { idling, hovering, pressing };

    void set_state(state_t next, typename Script::GameObject& obj)
    {
        if (state == next)
            return;

        state = next;

        if (! style)
            return;

        switch (state)
        {
            case idling:   style->idle(obj);  break;
            case hovering: style->hover(obj); break;
            case pressing: style->press(obj); break;
        }
    }

public:
    frog::gx2d::sprite normal;
    frog::gx2d::sprite hover;
    frog::gx2d::sprite press;
    frog::ptr<frog::button_action> action = nullptr;
    frog::ptr<frog::button_style_base<typename Script::GameObject>> style = nullptr;

    state_t state = idling;

    button_script_base(frog::ptr<frog::button_action> b_action = nullptr,
                       decltype(style) b_style = nullptr)
        : action(std::move(b_action)),
          style(std::move(b_style)) {}

    void init(typename Script::GameObject& obj, typename Script::Engine&) override
    {
        using namespace frog;

        if (style)
        {
            style->init(obj);
            style->idle(obj);
        }

        // TODO: Still not happy about this. It's strange to use the rectangle
        // from ui for button dimensions.
        if (obj.elements().empty())
            ui = obj.add_element(frog::mk_ptr<gx::ui_element>());
        else
            ui = obj.elements().front().get();
    }

    void frame_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    {
        using namespace frog;

        geo::vec2 pos = engine.camera_coords(engine.input->mouse());

        auto rect = ui->sprite.rect;
        bool collides = geo::is_collision(rect, pos);
        bool l_pressed = engine.input->mouse().but_l.pressed;
        bool l_released = engine.input->mouse().but_l.released;

        if (collides && l_pressed)
            down = true;

        if (l_released)
        {
            if (down && collides)
                if (action) action->action();
            down = false;
        }
        else if (down)
            if (action) action->frame_holding();

        if (down)
            set_state(pressing, obj);
        else if (collides)
            set_state(hovering, obj);
        else
            set_state(idling, obj);
    }

    void stable_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    {
        if (down)
            if (action) action->stable_holding();

        if (style)
            style->stable_update(obj);
    }

    bool is_down() const
    {
        return down;
    }
};


}      // namespace frog
