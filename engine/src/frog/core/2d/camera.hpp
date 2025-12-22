#pragma once

#include "frog/geometry/rectangle.hpp"

namespace frog
{

struct camera2d : public geo::rect
{
    geo::rect prev;

    camera2d(geo::vec2 pos, geo::vec2 size)
        : geo::rect(pos, size)
        , prev(pos, size)
    { }
};

} // namespace frog
