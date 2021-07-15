#pragma once

#include"graphics/include.hpp"

#include <stdexcept>


namespace frog::gl
{


class renderer
{
    void gl_settings();

public:
    renderer(int major, int minor, bool vsync = true);

    void viewport(int width, int height);

    void clear_color(float r, float g, float b);

    void clear();

    void prepare2D();

    void prepare3D();
};


} // namespace frog::gl
