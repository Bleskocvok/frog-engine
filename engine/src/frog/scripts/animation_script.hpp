#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/animation.hpp"

#include <string>
#include <utility>      // move


namespace frog {


class animation_script : public frog::script2d
{
    gx2d::animation animation;

public:
    animation_script(gx2d::animation animation)
        : animation(std::move(animation)) {}

    void init(frog::game_object2d&, frog::engine2d&) override
    {
    }

    void frame_update(frog::game_object2d&, frog::engine2d& eng) override
    {
        animation.update(eng.global->frame_time());
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d&) override
    {
        obj.model().tex = animation.frame().tex;
    }
};


}      // namespace frog
