#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/utils/exception.hpp"

#include <cstdint>      // uint8_t
#include <stdexcept>
#include <string_view>


namespace frog::gx
{


using rgba_t = geo::vec<std::uint8_t, 4>;


inline rgba_t color_to_255(geo::vec4 v)
{
    return { std::uint8_t( v.r() * 255.0f ), std::uint8_t( v.g() * 255.0f ),
             std::uint8_t( v.b() * 255.0f ), std::uint8_t( v.a() * 255.0f ) };
}


inline geo::vec4 rgb_to_vec( gx::rgba_t rgb )
{
    return { rgb.a() / 255.0f, rgb.g() / 255.0f, rgb.b() / 255.0f,
             rgb.a() / 255.0f };
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

namespace colors
{

inline const auto WHITE = rgba_t{ 255, 255, 255, 255 };
inline const auto BLACK = rgba_t{   0,   0,   0, 255 };
inline const auto RED   = rgba_t{ 255,   0,   0, 255 };
inline const auto GREEN = rgba_t{   0, 255,   0, 255 };
inline const auto BLUE  = rgba_t{   0,   0, 255, 255 };

}  // namespace colors


} // namespace frog::gx
