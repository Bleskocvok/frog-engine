#pragma once

#include "frog/core/script.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/core/engine2d.hpp"

#include <utility>      // move
#include <sstream>      // ostringstream
#include <iomanip>      // setw, setprecision


namespace frog {


struct fps_script : frog::script2d
{
    static constexpr float label_height = 0.03;

    frog::gx::ui_element* display = nullptr;

    frog::geo::vec2 pos = { -0.5, -0.5 };

    explicit fps_script(frog::geo::vec2 pos) : pos(pos) {}

    fps_script() = default;

    void init(frog::game_object2d& object, frog::engine2d&) override
    {
        using namespace frog;

        display = object.add_element(mk_ptr<gx::ui_element>());
        display->label = { "fps: -", 1 };
        display->pos() = pos + geo::vec2(0, label_height * 0.5);
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
