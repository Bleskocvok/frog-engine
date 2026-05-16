#pragma once

#include "frog/geometry/rectangle.hpp"

namespace frog {

struct camera2d : public geo::rect
{
    static constexpr geo::vec2 DEFAULT_POS = { geo::vec2{ 0 } };
    static constexpr geo::vec2 DEFAULT_SIZE = { geo::vec2{ 1 } };

    geo::rect prev;

    constexpr camera2d(geo::vec2 pos, geo::vec2 size)
        : geo::rect(pos, size)
        , prev(pos, size)
    { }

    constexpr void reset()
    {
        *this = camera2d{ DEFAULT_POS, DEFAULT_SIZE };
    }
};

} // namespace frog
