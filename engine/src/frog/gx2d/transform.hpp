#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/lib2d/structs.hpp"
#include "frog/lib2d/utils/surface_editor.hpp"
#include "frog/lib2d/window.hpp"

#include <concepts>
#include <utility>
#include <optional>
#include <string>

namespace frog::gx2d::trans {

using Surf = frog::lib2d::detail::Surface;

template<typename T>
concept Transform
    = requires (T x, Surf s)
{
    { x(std::move(s)) } -> std::same_as<Surf>;
};

template<Transform T>
Surf operator>>(Surf&& s, T&& t)
{
    return t(std::move(s));
}

inline Surf create(frog::geo::ivec2 size)
{
    return frog::lib2d::gx::window::make_surface(size.x(), size.y());
}

struct CopyRect
{
    frog::lib2d::utils::SurfaceReader& r;
    frog::geo::irect irect;
    std::optional<frog::geo::ivec2> dst_pos;

    CopyRect(frog::lib2d::utils::SurfaceReader& r,
            frog::geo::irect irect,
            std::optional<frog::geo::ivec2> dst_pos = {})
        : r(r)
        , irect(irect)
        , dst_pos(dst_pos)
    { }

    Surf operator()(Surf src);
};

inline Surf copy(frog::lib2d::utils::SurfaceReader& r,
                 std::optional<frog::geo::irect> irect = {})
{
    if (irect)
        return create(r.size())
            >> CopyRect(r, *irect);

    return create(r.size())
        >> CopyRect(r, { { 0 }, r.size() });
}

struct Outline
{
    int pixels = 0;
    frog::gx::rgba_t rgb;

    Outline(int pixels, frog::gx::rgba_t rgb = { 255, 255, 255, 255 })
        : pixels(pixels)
        , rgb(rgb)
    { }

    Surf operator()(Surf src);
};

static_assert(Transform<Outline>);
static_assert(Transform<CopyRect>);

template<typename Engine>
Surf copy_from_engine(Engine& eng, const std::string& tex,
                      std::optional<frog::geo::irect> irect = {})
{
    Surf out;

    eng.read_texture_file(tex, [&](frog::geo::ivec2,
                                  frog::lib2d::utils::SurfaceReader& r)
    {
        out = copy(r, irect);
    });

    return out;
}

} // namespace frog::lib2d::utils

#endif
