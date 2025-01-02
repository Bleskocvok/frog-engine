#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/graphics/color.hpp"

#include <string>


namespace frog::gx2d {


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
};


}  // namespace frog::2d
