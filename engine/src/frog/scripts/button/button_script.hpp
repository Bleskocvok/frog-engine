#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/collision.hpp"
#include "frog/graphics/ui_element.hpp"

#include "frog/debug.hpp"

#include "button_action.hpp"
#include "button_style.hpp"

#include <optional>
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

    std::optional<frog::geo::rect> default_rect;

    enum state_t { idling, hovering, pressing };

    bool input_from_engine = true;
    bool override_press = false;
    bool override_release = false;

    void set_state(state_t next, typename Script::GameObject& obj)
    {
        if (state == next)
            return;

        state = next;

        if (not  style)
            return;

        switch (state)
        {
            case idling:   style->idle(obj);  break;
            case hovering: style->hover(obj); break;
            case pressing: style->press(obj); break;
        }
    }

    void reset_override_input()
    {
        override_press = false;
        override_release = false;
    }

public:
    frog::gx2d::sprite normal;
    frog::gx2d::sprite hover;
    frog::gx2d::sprite press;
    frog::ptr<frog::button_action_base<typename Script::GameObject,
                                       typename Script::Engine>> action = nullptr;
    frog::ptr<frog::button_style_base<typename Script::GameObject>> style = nullptr;

    state_t state = idling;

    button_script_base(decltype(action) b_action = nullptr,
                       decltype(style) b_style = nullptr,
                       std::optional<frog::geo::rect> rect = std::nullopt)
        : default_rect(std::move(rect)),
          action(std::move(b_action)),
          style(std::move(b_style))
    {}

    void manually_activated(bool v)
    {
        input_from_engine = not v;
    }

    void send_press()
    {
        override_press = true;
    }

    void send_release()
    {
        override_release = true;
    }

    void reset()
    {
        reset_override_input();
        down = false;
    }

    geo::vec2 get_pos() const
    {
        return get_rect().pos;
    }

    void set_pos(geo::vec2 pos)
    {
        auto r = get_rect();
        r.pos = pos;
        set_rect(r);
    }

    geo::rect get_rect() const
    {
        if (not ui)
            return default_rect ? *default_rect : frog::geo::rect{};

        return ui->sprite.rect;
    }

    void set_rect(geo::rect rect)
    {
        if (not ui)
        {
            if (default_rect)
                *default_rect = rect;
            return;
        }

        ui->sprite.rect = rect;
    }

    void set_crop_top(float delta)
    {
        delta = std::max(0.0f, delta);

        if (not ui->sprite.crop)
            ui->sprite.crop.emplace();

        ui->sprite.crop->top = delta;
    }

    void set_crop_bot(float delta)
    {
        delta = std::max(0.0f, delta);

        if (not ui->sprite.crop)
            ui->sprite.crop.emplace();

        ui->sprite.crop->bot = delta;
    }

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

        if (default_rect)
            ui->sprite.rect = *default_rect;
    }

    void frame_update(typename Script::GameObject& obj,
                      typename Script::Engine& engine) override
    {
        using namespace frog;

        geo::vec2 pos = engine.mouse_pos_in_ui();

        auto rect = ui->sprite.rect;
        bool collides = geo::is_collision(rect, pos);
        bool l_pressed = engine.input->mouse().but_l.pressed;
        bool l_released = engine.input->mouse().but_l.released;

        if (not input_from_engine)
        {
            l_pressed = override_press;
            l_released = override_release;
        }

        reset_override_input();

        if (collides && l_pressed)
            down = true;

        if (l_released)
        {
            if (down && collides)
                if (action)
                {
                    action->action();
                    action->action(obj, engine);
                }
            down = false;
        }
        else if (down)
            if (action)
            {
                action->frame_holding();
                action->frame_holding(obj, engine);
            }

        if (down)
            set_state(pressing, obj);
        else if (collides)
            set_state(hovering, obj);
        else
            set_state(idling, obj);
    }

    void stable_update(typename Script::GameObject& obj,
                       typename Script::Engine& engine) override
    {
        if (down)
            if (action)
            {
                action->stable_holding();
                action->stable_holding(obj, engine);
            }

        if (style)
            style->stable_update(obj);
    }

    bool is_down() const
    {
        return down;
    }
};


}      // namespace frog
