#pragma once

#include "frog/geometry/rectangle.hpp"

#include <algorithm>        // max

namespace frog::gx2d {

struct Crop
{
    float top = 0;
    float bot = 0;
    float left = 0;
    float right = 0;
};

inline Crop clamp(Crop c)
{
    c.top = std::max(0.0f, c.top);
    c.bot = std::max(0.0f, c.bot);
    c.left = std::max(0.0f, c.left);
    c.right = std::max(0.0f, c.right);
    return c;
}

// Source + crop describe the are that should be cut out of dest. Therefore
// returns the crop to change dest to cropped src.
inline frog::gx2d::Crop multiply_crop(const frog::geo::rect& src,
                                      const frog::gx2d::Crop& crop,
                                      const frog::geo::rect& dest)
{
    frog::gx2d::Crop r;
    auto dif = src.pos - dest.pos;
    r.top   =  dif.y() + crop.top   - 0.5 * ( src.size.y() - dest.size.y() );
    r.bot   = -dif.y() + crop.bot   - 0.5 * ( src.size.y() - dest.size.y() );
    r.left  =  dif.x() + crop.left  - 0.5 * ( src.size.x() - dest.size.x() );
    r.right = -dif.x() + crop.right - 0.5 * ( src.size.x() - dest.size.x() );
    return frog::gx2d::clamp(r);
}

} // namespace frog::gx2d
