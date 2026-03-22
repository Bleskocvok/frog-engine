#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/utils/assert.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/collision.hpp"
#include "frog/graphics/ui_element.hpp"

#include "frog/debug.hpp"

#include "button_action.hpp"
#include "button_style.hpp"

#include <optional>
#include <utility>      // move
#include <vector>


namespace frog {


template<typename Script>
class button_script_base;


using button_script2d = button_script_base<frog::script2d>;
using button_script   = button_script_base<frog::script>;


template<typename Script>
class button_script_base : public Script
{
    using GameObject = typename Script::GameObject;
    using Engine = typename Script::Engine;

    frog::gx::ui_element* ui = nullptr;
    Script::GameObject* game_obj = nullptr;

    bool down = false;

    std::optional<frog::geo::rect> default_rect;

    enum state_t { idling, hovering, pressing };

    bool input_from_engine = true;
    bool override_press = false;
    bool override_release = false;

    bool is_invisible = false;

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

    void button_update(typename Script::GameObject& obj,
                      typename Script::Engine& engine)
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
        {
            down = true;
            press_down(obj, engine);
        }

        if (l_released)
        {
            if (down && collides)
                action(obj, engine);
            else if (not down && not collides)
                outside_press(obj, engine);

            down = false;
        }
        else if (down)
            frame_holding(obj, engine);

        if (down)
            set_state(pressing, obj);
        else if (collides)
            set_state(hovering, obj);
        else
            set_state(idling, obj);
    }

    template<typename Func>
    void for_each_action(Func&& f)
    {
        for (auto& a : actions)
            if (a)
                f(*a);
    }

    void action(GameObject& o, Engine& e)
    {
        for_each_action([&](Action& a){ a.action(); a.action(o, e); });
    }

    void press_down(GameObject& o, Engine& e)
    {
        for_each_action([&](Action& a){ a.press_down(); a.press_down(o, e); });
    }

    void outside_press(GameObject& o, Engine& e)
    {
        for_each_action([&](Action& a){ a.outside_press(); a.outside_press(o, e); });
    }

    void frame_holding(GameObject& o, Engine& e)
    {
        for_each_action([&](Action& a){ a.frame_holding(); a.frame_holding(o, e); });
    }

    void stable_holding(GameObject& o, Engine& e)
    {
        for_each_action([&](Action& a){ a.stable_holding(); a.stable_holding(o, e); });
    }

public:
    using Action = frog::button_action_base<typename Script::GameObject,
                                            typename Script::Engine>;

    frog::gx2d::Sprite normal;
    frog::gx2d::Sprite hover;
    frog::gx2d::Sprite press;
    std::vector<frog::ptr<Action>> actions;
    frog::ptr<frog::button_style_base<typename Script::GameObject>> style = nullptr;

    state_t state = idling;

    button_script_base(frog::ptr<Action> b_action = nullptr,
                       decltype(style) b_style = nullptr,
                       std::optional<frog::geo::rect> rect = std::nullopt)
        : default_rect(std::move(rect)),
          style(std::move(b_style))
    {
        if (b_action)
            actions.push_back(std::move(b_action));
    }

    void add_action(frog::ptr<Action> act)
    {
        actions.push_back(std::move(act));
    }

    template<typename T>
    T* get_action()
    {
        for (auto& a : actions)
        {
            if (!a)
                continue;

            auto* ptr = dynamic_cast<T*>(a.get());

            if (ptr)
                return ptr;
        }
        return nullptr;
    }

    bool initialized() const { return ui; }

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

        if (style)
        {
            frog_assert(game_obj);
            style->set_rect(*game_obj, rect);
        }

        ui->sprite.rect = rect;
    }

    void set_crop(gx2d::Crop crop)
    {
        if (not ui)
            return;

        crop.top = std::max(0.0f, crop.top);
        crop.bot = std::max(0.0f, crop.bot);
        crop.left = std::max(0.0f, crop.left);
        crop.right = std::max(0.0f, crop.right);

        if (style)
        {
            frog_assert(game_obj);
            style->set_crop(*game_obj, crop);
        }

        if (not ui->sprite.crop)
            ui->sprite.crop.emplace();

        ui->sprite.crop = crop;
    }

    gx2d::Crop get_crop() const
    {
        if (not ui)
            return {};
        return ui->sprite.crop.value_or(gx2d::Crop{});
    }

    void invisible(bool val)
    {
        is_invisible = val;
    }

    void init(typename Script::GameObject& obj, typename Script::Engine&) override
    {
        using namespace frog;

        // if (style)
        // {
        //     style->init(obj);
        //     style->idle(obj);
        // }

        // TODO: Still not happy about this. It's strange to use the rectangle
        // from ui for button dimensions.
        if (obj.elements().empty())
            ui = obj.add_element(frog::mk_ptr<gx::ui_element>());
        else
            ui = obj.elements().front().get();

        if (default_rect)
            ui->sprite.rect = *default_rect;

        if (style)
        {
            style->init(obj);
            style->idle(obj);
        }

        game_obj = &obj;
    }

    void frame_update([[maybe_unused]] typename Script::GameObject& obj,
                      [[maybe_unused]] typename Script::Engine& engine) override
    {}

    // void end_frame_update(typename Script::GameObject& obj,
    //                       typename Script::Engine&) override
    // {
    //     for (auto& ui : obj.elements())
    //         ui->hide = is_invisible;
    // }
    void end_update(typename Script::GameObject& obj,
                    typename Script::Engine&) override
    {
        for (auto& ui : obj.elements())
            ui->hide = is_invisible;
    }

    void stable_update(typename Script::GameObject& obj,
                       typename Script::Engine& engine) override
    {
        button_update(obj, engine);

        if (down)
            stable_holding(obj, engine);

        if (style)
            style->stable_update(obj);
    }

    bool is_down() const
    {
        return down;
    }
};


}      // namespace frog
