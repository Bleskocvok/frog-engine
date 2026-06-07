#pragma once

#include "frog/core/script.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/core/engine2d.hpp"

#include <cmath>
#include <utility>      // move
#include <sstream>      // ostringstream
#include <iomanip>      // setw, setprecision


namespace frog {


struct fps_script : frog::script2d
{
    static constexpr float label_height = 0.03;

    frog::gx::ui_element* display = nullptr;

    frog::geo::vec2 pos = { -0.5, -0.5 };

    double accum = 0;
    double actual_length = 0;
    int count = 0;

    explicit fps_script(frog::geo::vec2 pos) : pos(pos)
    { }

    fps_script() = default;

    void init(frog::game_object2d& object, frog::engine2d&) override
    {
        using namespace frog;

        display = object.add_element(mk_ptr<gx::ui_element>());
        display->label = { "fps: -", 1 };
        display->pos() = pos + geo::vec2(0, label_height * 0.5);
        display->size() = { 0, label_height };
    }

    void recalc(double fps)
    {
        auto out = std::ostringstream{};

        out << "fps: " << std::setw(7) << std::fixed << std::setprecision(2)
            << fps;

        display->label->str = std::move(out).str();

    }

    void frame_update(frog::game_object2d&, frog::engine2d& engine) override
    {
        count++;

        accum += engine.global->frame_time();
        actual_length += engine.global->frame_time();

        if (accum > 1)
        {
            recalc(count / actual_length);

            accum = std::fmod(accum, 1);
            count = 0;
            actual_length = 0;
        }
    }
};


}   // namespace frog
