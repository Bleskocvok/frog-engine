#pragma once

#include"graphics/include.hpp"

#include <stdexcept>


namespace frog::gl
{


class renderer
{
    os::window window;

    void gl_settings();

    void prepare2D();

    void prepare3D();

public:
    renderer(int major, int minor, bool vsync = true);

    void viewport(int width, int height);

    void clear_color(float r, float g, float b);

    void clear();

    void swap_buffers();
};


} // namespace frog::gl
