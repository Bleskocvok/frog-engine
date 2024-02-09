#pragma once

#include "vector.hpp"
#include "basic.hpp"        // Pi

#include <optional>

namespace frog::geo
{


class physics2d
{
    void verlet_solve();

public:
    using idx_t = unsigned;

    // collection of points, joints between them and angles
    struct collection {};

    struct angle
    {
        // b is middle, a and c are ends
        idx_t a;
        idx_t b;
        idx_t c;
        // radians
        float angle = Pi;
    };

    struct joint
    {
        idx_t a;
        idx_t b;
        float dist = 0;
    };

    struct point
    {
        geo::vec2 pos;
        geo::vec2 prev;
        float radius = 1;
        // 0 = infinite weight
        float inv_weight = 0;
    };

    struct settings
    {
        float gravity = 9.81;
        int iterations = 10;
    };

    physics2d(settings s) {}

    collection& collection_at(idx_t i);
    const collection& collection_at(idx_t i) const;

    idx_t add_collection();

    idx_t add_point(idx_t col, point pt);

    void add_joint(idx_t col, joint j);
};


} // namespace frog::geo
