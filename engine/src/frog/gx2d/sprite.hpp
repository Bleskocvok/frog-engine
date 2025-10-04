#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"

#include <string>


namespace frog::gx2d {

struct prev_sprite
{
    geo::vec2 pos;
    float angle = 0;
};

enum class Interpolation
{
    NONE,
    INTERPOLATE,
    EXTRAPOLATE
};

struct sprite
{
    std::string image_tag;
    geo::rect rect;

    gx::rgba_t color = gx::colors::white;
    float angle = 0;
    bool flipped = false;

    geo::rect tex = { 0, 0, 1, 1 };

    // lower layer = rendered earlier
    unsigned layer = 0;

    prev_sprite prev;
    Interpolation interpolation = Interpolation::NONE;
};


inline void calculate_prev(sprite& sprite)
{
    sprite.prev.pos = sprite.rect.pos;
    sprite.prev.angle = sprite.angle;
}


}  // namespace frog::2d
