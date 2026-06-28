#pragma once

#include "idx.hpp"

#include "frog/geometry/vector.hpp"
#include "frog/geometry/basic.hpp"        // Pi

namespace frog::geo {

struct Angle
{
    // b is middle, a and c are ends
    idx_t a = 0;
    idx_t b = 0;
    idx_t c = 0;
    // My past self was probably thinking optimization, using radians is
    // faster (sin/cos use them, so no need for conversion).
    // radians
    float angle = Pi;
};

struct Joint
{
    idx_t a = 0;
    idx_t b = 0;
    float dist = 0;
};

struct Point
{
    geo::vec2 pos;
    geo::vec2 prev;
    float radius = 1;
    // 0 = infinite weight
    float inv_weight = 1;
    bool bound_by_universum = true;

    unsigned flying = 0;
};

} // namespace frog::geo
