#pragma once

#include "frog/scripts/button/button_style.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/gx2d/animation.hpp"

#include <utility>      // move

namespace frog {

struct animation_style : frog::button_style2d
{
    frog::gx::ui_element* ui = nullptr;
    bool activated = false;
    frog::gx2d::animation anim;
    float update_delta = 0;

    animation_style(frog::gx2d::animation anim, float update_delta)
        : anim(std::move(anim)),
          update_delta(update_delta)
    { }

    void init(frog::game_object2d& obj) override
    {
        using namespace frog;
        if (obj.elements().empty())
            ui = obj.add_element(frog::mk_ptr<gx::ui_element>());
        else
            ui = obj.elements().front().get();

        auto rect = ui->sprite.rect;
        ui->sprite = anim.frame();
        ui->sprite.rect = rect;

        apply();
    }

    void stable_update(frog::game_object2d&) override
    {
        // TODO: Switch between a selection of animations.
        // Cannot access engine here, so gotta use stored delta.
        anim.update(update_delta);
        apply();
    }

    void apply()
    {
        ui->sprite.tex = anim.frame().tex;
    }
};

} // namespace frog
