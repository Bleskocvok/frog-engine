#pragma once

#include "geometry/rectangle.hpp"

namespace frog::gx2d {


struct sprite
{
    std::string image_tag;
    float opacity = 1;
    // TODO: color

    geo::rect rect;
    float rot = 0;

    // lower layer = rendered earlier
    unsigned layer = 0;
};


}  // namespace frog::2d
