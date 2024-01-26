#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "graphics/renderer_base.hpp"

#include <stdexcept>


namespace frog::gx {


class renderer2d : public frog::gx::renderer_base
{
public:
    renderer2d() = default;

    void viewport(int width, int height) override {}

    void clear_color(float r, float g, float b) override {}

    void clear() override {}

    void prepare2D() override {}

    void prepare3D() override {}
};


} // namespace frog::gl

#endif
