#pragma once

#include "frog/geometry/vector.hpp"

#include <cstdint>      // uint8_t


namespace frog::gx
{


using rgba_t = geo::vec<std::uint8_t, 4>;


inline rgba_t color_to_255(geo::vec4 v)
{
    return { std::uint8_t( v.r() * 255.0f ), std::uint8_t( v.g() * 255.0f ),
             std::uint8_t( v.b() * 255.0f ), std::uint8_t( v.a() * 255.0f ) };
}


namespace colors
{

inline const auto white = rgba_t{ 255, 255, 255, 255 };
inline const auto black = rgba_t{ 0, 0, 0, 255 };

}  // namespace colors


} // namespace frog::gx
