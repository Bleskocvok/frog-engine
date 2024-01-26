#pragma once


namespace frog::gx {


class renderer_base
{
public:
    virtual ~renderer_base() = default;

    virtual void viewport(int width, int height) = 0;

    virtual void clear_color(float r, float g, float b) = 0;

    virtual void clear() = 0;

    virtual void prepare2D() = 0;

    virtual void prepare3D() = 0;
};


} // namespace frog::gl
