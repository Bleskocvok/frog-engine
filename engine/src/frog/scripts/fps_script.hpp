#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/vector.hpp"

#include <string>
#include <utility>      // move
#include <sstream>      // ostringstream
#include <iomanip>      // setw, setprecision


namespace frog {


struct fps_script : frog::script2d
{
    static constexpr float label_height = 0.03;

    frog::gx::ui_element* display = nullptr;

    void init(frog::game_object2d& object, frog::engine2d&) override
    {
        using namespace frog;

        display = object.add_element(mk_ptr<gx::ui_element>());
        display->label = { "fps: -", 1 };
        display->pos() = geo::vec2(-0.5, -0.5 + label_height * 0.5);
        display->size() = { label_height };
    }

    void frame_update(frog::game_object2d&, frog::engine2d& engine) override
    {
        auto out = std::ostringstream{};

        out << "fps: " << std::setw(7) << std::fixed << std::setprecision(2)
            << engine.global->fps();

        display->label->str = std::move(out).str();
    }
};


}   // namespace frog
