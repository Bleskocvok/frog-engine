#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/paddle.hpp"

#include "bounce_script.hpp"
#include "constants.hpp"

#include <string>



struct fps_script : frog::script
{
    static constexpr float label_height = 0.03;

    frog::gx::ui_element* display = nullptr;

    void init(frog::game_object& object, frog::engine&) override
    {
        using namespace frog;

        display = object.add_element(mk_ptr<gx::ui_element>());
        display->label = { "fps: -", 1 };
        display->pos() = { -1, 1 - label_height };
        display->size() = { label_height };
        display->color() = { 0, 0, 0, 1 };
    }

    void frame_update(frog::game_object&, frog::engine& engine) override
    {
        display->label->str = "fps: " + std::to_string(engine.global->fps());
    }
};
