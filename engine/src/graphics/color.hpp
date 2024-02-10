#pragma once

#include "geometry/vector.hpp"

#include <cstdint>      // uint8_t


namespace frog::gx
{


using rgba_t = geo::vec<std::uint8_t, 4>;


inline rgba_t color_to_255(geo::vec4 v)
{
    return { v.r() * 255, v.g() * 255, v.b() * 255, v.a() * 255 };
}


} // namespace frog::gx
