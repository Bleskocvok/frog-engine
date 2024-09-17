#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/vector.hpp"

#include <string>


struct fps_script : frog::script2d
{
    static constexpr float label_height = 0.03;

    frog::gx::ui_element* display = nullptr;

    void init(frog::game_object2d& object, frog::engine2d&) override
    {
        using namespace frog;

        display = object.add_element(mk_ptr<gx::ui_element>());
        display->label = { "fps: -", 1 };
        display->pos = geo::vec2(-0.5, -0.5 + label_height * 0.5);
        display->size = { label_height };
    }

    void frame_update(frog::game_object2d&, frog::engine2d& engine) override
    {
        display->label->str = "fps: " + std::to_string(engine.global->fps());
    }
};
