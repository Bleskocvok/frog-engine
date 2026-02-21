#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/animation.hpp"

#include <utility>      // move


namespace frog {


class animation_script : public frog::script2d
{
    gx2d::Animation animation_;

    frog::gx::ui_element* ui = nullptr;

    gx2d::Sprite& sprite(frog::game_object2d& obj)
    {
        return ui == nullptr ? obj.model() : ui->sprite;
    }

public:
    animation_script(gx2d::Animation animation_, frog::gx::ui_element* ui = nullptr)
        : animation_(std::move(animation_)) {}

    void init(frog::game_object2d& obj, frog::engine2d&) override
    {
        apply(obj);
    }

    void frame_update(frog::game_object2d&, frog::engine2d& eng) override
    {
        animation_.update(eng.global->frame_time());
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d&) override
    {
        apply(obj);
    }

    void apply(frog::game_object2d& obj)
    {
        sprite(obj).tex = animation_.frame().tex;
    }

    const gx2d::Animation& animation() const { return animation_; }
          gx2d::Animation& animation()       { return animation_; }
};


}      // namespace frog
