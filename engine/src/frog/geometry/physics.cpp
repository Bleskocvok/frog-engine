#include "physics.hpp"

#include <utility>          // pair
#include <cstddef>          // size_t
#include <algorithm>        // max, min, clamp
#include <vector>


namespace frog::geo {


template<typename T>
class optimization_grid
{
    std::vector<std::vector<T>> grid;
    ivec2 dim;
    rect box;
    vec2 chunk;

    template<typename Func>
    void for_each_around_impl(circle c, Func f)
    {
        using std::min, std::max;

        auto inside = c.pos - box.top_left();
        auto minim = ivec2{ int( ( inside.x() - c.radius ) / chunk.x() ),
                            int( ( inside.y() - c.radius ) / chunk.y() ) };
        auto maxim = ivec2{ int( ( inside.x() + c.radius ) / chunk.x() ),
                            int( ( inside.y() + c.radius ) / chunk.y() ) };

        for     (int y = max( 0, minim.y() ); y <= min( dim.y() - 1, maxim.y() ); ++y)
            for (int x = max( 0, minim.x() ); x <= min( dim.x() - 1, maxim.x() ); ++x)
                if (is_collision(c, rect_at(x, y)))
                    f(x, y);
    }

public:
    optimization_grid(ivec2 dim, rect box)
        : grid(dim.x() * dim.y()),
          dim(dim),
          box(box),
          chunk(box.size.x() / dim.x(), box.size.y() / dim.y()) {}

    const auto& at(int x, int y) const { return grid.at( x + y * dim.x() ); }
          auto& at(int x, int y)       { return grid.at( x + y * dim.x() ); }

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


void soft_physics2d::apply_inertia(point& pt, float delta)
{
    vec2 diff = pt.pos - pt.prev;
    pt.prev = pt.pos;
    pt.pos += diff * settings_.inertia;
    pt.pos.y() += settings_.gravity * delta * delta;
}

void soft_physics2d::encapsulate(point& pt, rect rect)
{
    rect.size *= 0.5;
    pt.pos.x() = std::clamp(pt.pos.x(), rect.pos.x() - rect.size.x() + pt.radius,
                                        rect.pos.x() + rect.size.x() - pt.radius);
    pt.pos.y() = std::clamp(pt.pos.y(), rect.pos.y() - rect.size.y() + pt.radius,
                                        rect.pos.y() + rect.size.y() - pt.radius);
}

void soft_physics2d::solve_collision(point& a, point& b)
{
    vec2 dif = a.pos - b.pos;
    float dist = dif.length();
    float radii = a.radius + b.radius;

    if (dist < radii)
    {
        // TODO: avoid division by 0
        float ratio = (dist - radii) / (dist * (a.inv_weight * b.inv_weight));
        a.pos -= dif * (ratio * a.inv_weight * 0.5);
        b.pos += dif * (ratio * b.inv_weight * 0.5);
    }
}

void soft_physics2d::solve_joint(joint j)
{
    auto& a = points_[ map_points[ j.a ] ].second;
    auto& b = points_[ map_points[ j.b ] ].second;

    vec2 dif = a.pos - b.pos;
    float dist = dif.length();
    float desired = j.dist;

    float ratio = (dist - desired) / (dist * (a.inv_weight * b.inv_weight));
    a.pos -= dif * (ratio * a.inv_weight * 0.5);
    b.pos += dif * (ratio * b.inv_weight * 0.5);
}

// TODO: there must be something wrong, doesn't seem to work for angles ~180 deg
// probably has to do with angles being (mod 360)
// probably just use geo::angle_diff
void soft_physics2d::solve_angle(angle alpha)
{
    auto& a = points_[ map_points[ alpha.a ] ].second;
    auto& b = points_[ map_points[ alpha.b ] ].second;
    auto& c = points_[ map_points[ alpha.c ] ].second;

    float cur = vec_angle(a.pos - b.pos, c.pos - b.pos);
    float desired = alpha.angle;
    float dif = angle_diff(cur, desired);

    a.pos -= b.pos;
    c.pos -= b.pos;

    polar2 pol_a = to_polar(a.pos);
    polar2 pol_c = to_polar(c.pos);

    float ratio = dif / (a.inv_weight * b.inv_weight);
    pol_a.theta -= (ratio * a.inv_weight * 0.5);
    pol_c.theta += (ratio * b.inv_weight * 0.5);

    a.pos = to_cartesian(pol_a);
    c.pos = to_cartesian(pol_c);
    a.pos += b.pos;
    c.pos += b.pos;
}

void soft_physics2d::verlet_solve()
{
    for (auto& [idx, pt] : points_)
        apply_inertia(pt, settings_.delta);

    auto grid = optimization_grid<std::pair<idx_t, point*>>(
                                    settings_.grid_dim, settings_.universum);

    for (int it = 0; it < settings_.iterations; ++it)
    {
        for (auto& [i, pt] : points_)
            encapsulate(pt, settings_.universum);

        grid.clear();

        for (auto& [i, pt] : points_)
            grid.add_to_touching(circle(pt.pos, pt.radius), { i, &pt });

        for (auto& [i, pt] : points_)
            grid.for_each_around(circle(pt.pos, pt.radius), [&](std::pair<idx_t, point*> other)
                {
                    if (other.first > i)
                        solve_collision(pt, *other.second);
                });

        for (auto&[idx, j] : joints_)
            solve_joint(j);

        for (auto&[idx, a] : angles_)
            solve_angle(a);
    }
}

void soft_physics2d::update_maps()
{
    updated = last;

    map_points.clear();
    map_joints.clear();

    map_points.reserve(points_.size());
    map_joints.reserve(joints_.size());

    for (std::size_t i = 0; i < points_.size(); ++i)
        map_points.emplace(points_[i].first, i);

    for (std::size_t i = 0; i < joints_.size(); ++i)
        map_joints.emplace(joints_[i].first, i);
}


}  // namespace frog::geo
