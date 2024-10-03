#pragma once

#include "frog/geometry/rectangle.hpp"

#include <string>


namespace frog::gx2d {


struct sprite
{
    std::string image_tag;
    geo::rect rect;

    geo::vec4 color = { 1 };
    float angle = 0;
    bool flipped = false;

    geo::rect tex = { 0.5, 0.5, 1, 1 };

    // lower layer = rendered earlier
    unsigned layer = 0;
};


}  // namespace frog::2d
