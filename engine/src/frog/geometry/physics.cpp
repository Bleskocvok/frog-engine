#include "physics.hpp"

#include "circle.hpp"
#include "collision.hpp"
#include "frog/geometry/vector.hpp"
#include "polar.hpp"
#include "general.hpp"

#include "frog/debug.hpp"

#include <cassert>
#include <utility>          // pair
#include <algorithm>        // max, min, clamp, find, for_each
#include <vector>

namespace frog::geo {

void soft_physics2d::apply_inertia(point& pt, float delta)
{
    vec2 diff = pt.pos - pt.prev;
    pt.prev = pt.pos;
    pt.pos += diff * settings_.inertia;
    pt.pos += settings_.gravity * delta * delta;
}

bool soft_physics2d::encapsulate(point& pt, rect rect)
{
    if (not pt.bound_by_universum)
        return false;

    auto prev = pt.pos;

    rect.size *= 0.5;
    pt.pos.x() = std::clamp(pt.pos.x(), rect.pos.x() - rect.size.x() + pt.radius,
                                        rect.pos.x() + rect.size.x() - pt.radius);
    pt.pos.y() = std::clamp(pt.pos.y(), rect.pos.y() - rect.size.y() + pt.radius,
                                        rect.pos.y() + rect.size.y() - pt.radius);

    return pt.pos != prev;
}

bool soft_physics2d::solve_collision(point& a, point& b)
{
    vec2 dif = a.pos - b.pos;
    float dist = dif.length();
    float radii = a.radius + b.radius;

    if (dist < radii)
    {
        float div = dist * (a.inv_weight + b.inv_weight);

        // TODO: Think further about this code and see if it makes sense.
        if (div == 0) [[unlikely]]
        {
            dif.x() = radii;
            a.pos -= dif * 0.5;
            b.pos += dif * 0.5;
        }
        else [[likely]]
        {
            float ratio = (dist - radii) / div;
            a.pos -= dif * (ratio * a.inv_weight);
            b.pos += dif * (ratio * b.inv_weight);
        }
        return true;
    }

    return false;
}

void soft_physics2d::solve_joint(joint j)
{
    auto& a = point_at(j.a);
    auto& b = point_at(j.b);

    vec2 dif = a.pos - b.pos;
    float dist = dif.length();
    float desired = j.dist;
    float div = dist * (a.inv_weight + b.inv_weight);

    if (div == 0) [[unlikely]]
    {
        dif.x() = desired;
        a.pos -= dif * 0.5;
        b.pos += dif * 0.5;
    }
    else [[likely]]
    {
        float ratio = (dist - desired) / div;
        a.pos -= dif * (ratio * a.inv_weight);
        b.pos += dif * (ratio * b.inv_weight);
    }
}

// TODO: there must be something wrong, doesn't seem to work for angles ~180 deg
// probably has to do with angles being (mod 360)
// probably just use geo::angle_diff
void soft_physics2d::solve_angle(angle alpha)
{
    auto& a = point_at(alpha.a);
    auto& b = point_at(alpha.b);
    auto& c = point_at(alpha.c);

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

void soft_physics2d::calculate_grid()
{
    if (grid)
        grid->clear();
    else
        grid.emplace(settings_.grid_dim, settings_.universum);

    for (auto& [i, pt] : points())
        grid->add_to_touching(circle(pt.pos, pt.radius), { i, &pt });
}

void soft_physics2d::verlet_solve()
{
    for (auto& [idx, pt] : points())
        apply_inertia(pt, settings_.delta);

    // auto grid = optimization_grid<std::pair<idx_t, point*>>(
    //                                 settings_.grid_dim, settings_.universum);

    std::for_each(plugins_.begin(), plugins_.end(), [](auto& p){ p->before(); });

    for (int it = 0; it < settings_.iterations; ++it)
    {
        std::for_each(plugins_.begin(), plugins_.end(), [&](auto& p){ p->solve_all(it, points()); });

        for (auto& [i, pt] : points())
        {
            if (encapsulate(pt, settings_.universum))
                collisions_.insert(CollisionInfo{ i, BOUNDS });
        }

        // grid.clear();

        // for (auto& [i, pt] : points())
        //     grid.add_to_touching(circle(pt.pos, pt.radius), { i, &pt });

        if (not grid)
            calculate_grid();

        assert(grid);

        for (auto& elem : points())
        {
            auto& i = elem.first;
            auto& pt = elem.second;

            grid->for_each_around(circle(pt.pos, pt.radius), [&](std::pair<idx_t, point*> other)
                {
                    if (other.first <= i)
                        return;

                    if (solve_collision(pt, *other.second))
                        collisions_.insert(CollisionInfo{ i, other.first });
                });
        }

        for (auto&[idx, j] : joints())
            solve_joint(j);

        for (auto&[idx, a] : angles())
            solve_angle(a);

        calculate_grid();
    }

    std::for_each(plugins_.begin(), plugins_.end(), [](auto& p){ p->after(); });
}

void soft_physics2d::remove()
{
    // TODO: Consider how this could be made more efficient.
    if (not points_removal.empty())
    {
        auto has = [](const auto& cont, const auto& elem)
        {
            return std::find(cont.begin(), cont.end(), elem) != cont.end();
        };

        for (unsigned i = 0; i < joints_.size(); ++i)
            if (has(points_removal, joints_.data[i].second.a)
             || has(points_removal, joints_.data[i].second.b))
                joints_removal.push_back(joints_.data[i].first);

        // TODO: Do the same thing for angles;
    }

    points_.remove(points_removal);
    joints_.remove(joints_removal);
    angles_.remove(angles_removal);

    points_removal.clear();
    joints_removal.clear();
    angles_removal.clear();
}

void soft_physics2d::push(idx_t point, vec2 delta)
{
    point_at(point).prev -= delta;
}

void soft_physics2d::explode(circle c, float power)
{
    for_each_colliding(c, [&](auto idx, point* pt)
    {
        vec2 dif = pt->pos - c.pos;
        float len = dif.length();
        float dist = c.radius == 0 || len == 0
                   ? 1.0
                   : 1 - len / c.radius;
        dif /= len;
        push(idx, dif * dist * power);
    });
}

}  // namespace frog::geo
