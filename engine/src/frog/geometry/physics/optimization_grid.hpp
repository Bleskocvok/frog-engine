#pragma once

#include "frog/geometry/circle.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/vector.hpp"

#include <algorithm>        // max, min, clamp, find
#include <vector>
#include <algorithm>

namespace frog::geo {

template<typename T>
class optimization_grid
{
    std::vector<std::vector<T>> grid;
    ivec2 dim;
    rect box;
    vec2 chunk;
    vec2 inv_chunk;

    template<typename Func>
    void for_each_around_impl(circle c, Func f)
    {
        using std::min, std::max;

        const auto inside = c.pos - box.top_left();
        const auto minim = ivec2{ int( ( inside.x() - c.radius ) * inv_chunk.x() ),
                                  int( ( inside.y() - c.radius ) * inv_chunk.y() ) };
        const auto maxim = ivec2{ int( ( inside.x() + c.radius ) * inv_chunk.x() ),
                                  int( ( inside.y() + c.radius ) * inv_chunk.y() ) };

        const auto start_y = max( 0, minim.y() );
        const auto end_y = min( dim.y() - 1, maxim.y() );
        const auto start_x = max( 0, minim.x() );
        const auto end_x = min( dim.x() - 1, maxim.x() );

        for     (int y = start_y; y <= end_y; ++y)
            for (int x = start_x; x <= end_x; ++x)
                // This appears to be slower.
                // if (is_collision(c, rect_at(x, y)))
                    f(x, y);
    }

    // const auto& at(int x, int y) const { return grid.at( x + y * dim.x() ); }
    //       auto& at(int x, int y)       { return grid.at( x + y * dim.x() ); }

    const auto& at(int x, int y) const { return grid[ x + y * dim.x() ]; }
          auto& at(int x, int y)       { return grid[ x + y * dim.x() ]; }

public:
    optimization_grid(ivec2 dim, rect box)
        : grid(dim.x() * dim.y()),
          dim(dim),
          box(box),
          chunk(box.size.x() / dim.x(), box.size.y() / dim.y())
    {
        inv_chunk = 1.0 / chunk;
    }

    const auto& dimensions() const { return dim; }

    rect rect_at(int x, int y) const
    {
        auto pos = box.top_left() + chunk * 0.5 + vec2{ x * chunk.x(), y * chunk.y() };
        return rect{ pos, chunk };
    }

    void add_to_touching(circle c, T val)
    {
        for_each_around_impl(c, [&](int x, int y){ at(x, y).push_back(val); });
    }

    template<typename Func>
    void for_each_around(circle c, Func f)
    {
        for_each_around_impl(c, [&](int x, int y)
        {
            for (auto& val : at(x, y))
                f(val);
        });
    }

    void clear()
    {
        for (auto& square : grid)
        {
            square.clear();
        }
    }
};

} // frog::geo
