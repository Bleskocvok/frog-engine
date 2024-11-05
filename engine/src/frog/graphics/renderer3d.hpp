#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "frog/gx3d/include.hpp"
#include "frog/graphics/renderer_base.hpp"

#include <stdexcept>


namespace frog::gl {


class renderer3d : public frog::gx::renderer_base
{
    void gl_settings();

public:
    renderer3d(int major, int minor, bool vsync = true);

    void viewport(int width, int height) override;

    void clear_color(float r, float g, float b) override;

    void clear() override;

    void prepare2D() override;

    void prepare3D() override;
};


} // namespace frog::gl

#endif
