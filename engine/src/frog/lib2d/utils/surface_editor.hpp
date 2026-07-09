#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/utils/assert.hpp"
#include "frog/lib2d/structs.hpp"

#include "frog/lib2d/sdl_include.hpp"
#include SDL_HEADER

#include <cstdint>

namespace frog::lib2d::utils {

class SurfaceEditor
{
    SDL_Surface* surface = nullptr;
    int w = 0;
    int h = 0;

    constexpr static int BytesPerPixel = 4;

    std::uint32_t* pixel_pos(frog::geo::ivec2 p);

public:
    SurfaceEditor(frog::lib2d::detail::Surface& surface_);

    ~SurfaceEditor();

    frog::gx::rgba_t read_pixel(frog::geo::ivec2 p);

    void write_pixel(frog::geo::ivec2 p, frog::gx::rgba_t c);
};

class SurfaceReader
{
    SurfaceEditor editor;

public:
    SurfaceReader(frog::lib2d::detail::Surface& surface_)
        : editor(surface_)
    { }

    frog::gx::rgba_t read_pixel(frog::geo::ivec2 p)
    {
        return editor.read_pixel(p);
    }
};

} // namespace frog::lib2d::utils

#endif
