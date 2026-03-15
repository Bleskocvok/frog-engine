#pragma once

#include "frog/core/engine2d.hpp"
#include "frog/core/script.hpp"
#include "frog/graphics/color.hpp"

namespace frog::scripts {

// TODO: Make templated, for 3D too.
class viewport2d : public script2d
{
    float ratio = 1;
    frog::gx::rgba_t clear_color;

public:
    viewport2d(float ratio = 1, frog::gx::rgba_t clear_color = { 0, 0, 0, 255 })
        : ratio(ratio)
        , clear_color(clear_color)
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
        float k = ratio;
        if (h / float(w) < k)
            eng.camera().size = { k * w / float(h), k };
        else
            eng.camera().size = { 1, h / float(w) };

        // TODO: This becomes immediate change, perhaps give an option to make
        // it interpolated.
        eng.camera().prev.size = eng.camera().size;
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
