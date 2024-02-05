#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "graphics/renderer_base.hpp"
#include "lib2d/window.hpp"

#include <stdexcept>


namespace frog::gx {


class renderer2d : public frog::gx::renderer_base
{
    frog::lib2d::gx::window* win_ptr = nullptr;
public:
    renderer2d(frog::lib2d::gx::window* window)
        : win_ptr(window)
    { }

    void viewport(int width, int height) override
    {
        win_ptr->update_size(width, height);
    }

    void clear_color(float r, float g, float b) override
    {
        win_ptr->clear_color(r / 255, g / 255, b / 255, 255);
    }

    void clear() override
    {
        win_ptr->clear();
    }

    void prepare2D() override {}

    void prepare3D() override {}
};


} // namespace frog::gl

#endif
