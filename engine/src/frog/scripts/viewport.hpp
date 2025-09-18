#pragma once

#include "frog/core/engine2d.hpp"
#include "frog/core/script.hpp"
#include "frog/graphics/color.hpp"

namespace frog::scripts {

// TODO: Make templated, for 3D too.
class viewport2d : public script2d
{
    frog::gx::rgba_t clear_color;

public:
    viewport2d(frog::gx::rgba_t clear_color = { 0, 0, 0, 255 })
        : clear_color(clear_color)
    { }

    void init(frog::game_object2d&, frog::engine2d& engine) override
    {
        using namespace frog;

        const auto& c = clear_color;
        engine.win_raw->clear_color(c.r(), c.g(), c.b(), c.a());

        auto size = engine.window_size();
        resize(size.x(), size.y(), engine);
    }

    virtual void resize(int w, int h, frog::engine2d& eng)
    {
        if (w > h)
            eng.camera().size = { w / float(h), 1 };
        else
            eng.camera().size = { 1, h / float(w) };
    }

    void stable_update(frog::game_object2d&, frog::engine2d& engine) override
    {
        if (engine.input->has_resized())
        {
            auto [w, h] = engine.input->has_resized().value();
            resize(w, h, engine);
        }

        if (engine.scenes->just_switched())
        {
            auto size = engine.window_size();
            resize(size.x(), size.y(), engine);
        }
    }
};

} // namespace frog::scripts
