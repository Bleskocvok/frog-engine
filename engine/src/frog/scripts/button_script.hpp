#pragma once

#include "frog/core/script.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/collision.hpp"

#include "button_action.hpp"

#include <string>
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

public:
    frog::gx2d::sprite normal;
    frog::gx2d::sprite hover;
    frog::gx2d::sprite press;
    frog::ptr<frog::button_action> action = nullptr;

    button_script_base(frog::gx2d::sprite normal, frog::gx2d::sprite hover,
                       frog::gx2d::sprite press, frog::ptr<frog::button_action> action = nullptr)
        : normal(std::move(normal)),
          hover(std::move(hover)),
          press(std::move(press)),
          action(std::move(action)) {}

    void init(typename Script::GameObject& obj, typename Script::Engine&) override
    {
        using namespace frog;

        // TODO: Refactor this.
        if (obj.elements().empty())
            ui = obj.add_element(mk_ptr<gx::ui_element>());
        else
            ui = obj.elements().front().get();

        ui->sprite = normal.image_tag;
    }

    void frame_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    {
        using namespace frog;

        geo::vec2 pos = engine.camera_coords(engine.input->mouse());

        auto rect = geo::rect(ui->pos, ui->size);
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
            ui->sprite = press.image_tag;
        else if (collides)
            ui->sprite = hover.image_tag;
        else
            ui->sprite = normal.image_tag;
    }

    void stable_update(typename Script::GameObject& obj, typename Script::Engine& engine) override
    {
        if (down)
            if (action) action->stable_holding();
    }

    bool is_down() const {
        return down;
    }
};


}      // namespace frog
