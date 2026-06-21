#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/utils/assert.hpp"

#include <algorithm>
#include <cmath>        // fabs, fmod
#include <cstdint>      // uint8_t
#include <stdexcept>
#include <string_view>


namespace frog::gx
{


using rgba_t = geo::vec<std::uint8_t, 4>;


inline rgba_t vec_to_rgb(geo::vec4 v)
{
    auto clamp = [](auto& x){ x = std::clamp<float>(x, 0, 1); };
    clamp( v.r() );
    clamp( v.g() );
    clamp( v.b() );
    clamp( v.a() );
    return { std::uint8_t( v.r() * 255.0f ), std::uint8_t( v.g() * 255.0f ),
             std::uint8_t( v.b() * 255.0f ), std::uint8_t( v.a() * 255.0f ) };
}


inline geo::vec4 rgb_to_vec( gx::rgba_t rgb )
{
    geo::vec4 vec = { float(rgb.r()), float(rgb.g()), float(rgb.b()), float(rgb.a()) };
    vec /= 255;
    return vec;
}


inline rgba_t rgb_multiply( gx::rgba_t a, gx::rgba_t b )
{
    return vec_to_rgb( rgb_to_vec( a ) * rgb_to_vec( b ) );
}


inline geo::vec4 vec_to_hsv(geo::vec4 vec)
{
    auto x_max = geo::max( vec.xyz() );
    auto x_min = geo::min( vec.xyz() );
    auto value = x_max;
    auto chroma = x_max - x_min;

    auto get_hue = [&]() -> float
    {
        auto c = chroma;

        if (c == 0)
            return 0;

        if (value == vec.r())
            return 60 * std::fmod( ( vec.g() - vec.b() ) / c, 6 );

        if (value == vec.g())
            return 60 * ( 2 + ( vec.b() - vec.r() ) / c );

        if (value == vec.b())
            return 60 * ( 4 + ( vec.r() - vec.g() ) / c );

        frog_assert(false);
    };

    auto hue = get_hue();
    auto saturation = value == 0 ? 0 : chroma / value;

    auto hsv = geo::vec4(hue, saturation, value, vec.a());
    return hsv;
}


inline geo::vec4 hsv_to_vec(geo::vec4 hsv)
{
    auto hue = hsv.x();         // [0, 360]
    auto saturation = hsv.y();  // [0, 1]
    auto value = hsv.z();       // [0, 1]

    auto chroma = value * saturation;

    auto h = hue / 60;

    float x = chroma * (1 - std::fabs(std::fmod(h, 2) - 1));

    auto table = [&]() -> geo::vec3
    {
        auto c = chroma;
        if (h < 1) return { c, x, 0 };
        if (h < 2) return { x, c, 0 };
        if (h < 3) return { 0, c, x };
        if (h < 4) return { 0, x, c };
        if (h < 5) return { x, 0, c };
        if (h < 6) return { c, 0, x };
        frog_assert(false);
    };

    auto m = value - chroma;
    auto rgb = table();

    // Adds it to all three components.
    rgb += m;

    auto vec = geo::vec4(rgb, hsv.a());
    return vec;
}


namespace color {

inline constexpr rgba_t from_hex(std::string_view str)
{
    using error = std::runtime_error;

    if (str.starts_with('#'))
        str.remove_prefix(1);

    if (str.size() != 6)
        throw error("from_hex: string.size() != 6");

    constexpr std::string_view hexes_up  = "0123456789ABCDEF";
    constexpr std::string_view hexes_low = "0123456789abcdef";

    auto find = [&](auto c)
    {
        auto idx = hexes_up.find(c);
        if (idx != hexes_up.npos)
            return idx;

        idx = hexes_low.find(c);
        if (idx != hexes_low.npos)
            return idx;

        throw error("from_hex: invalid string");
    };

    auto get_color = [&]()
        -> std::uint8_t
    {
        auto i = find(str[0]);
        auto j = find(str[1]);
        str.remove_prefix(2);
        return i * 16 + j;
    };

    auto r = get_color();
    auto g = get_color();
    auto b = get_color();
    return { r, g, b, 255 };
}

} // namespace color

namespace colors {

inline const auto WHITE = rgba_t{ 255, 255, 255, 255 };
inline const auto BLACK = rgba_t{   0,   0,   0, 255 };
inline const auto RED   = rgba_t{ 255,   0,   0, 255 };
inline const auto GREEN = rgba_t{   0, 255,   0, 255 };
inline const auto BLUE  = rgba_t{   0,   0, 255, 255 };

}  // namespace colors


} // namespace frog::gx
