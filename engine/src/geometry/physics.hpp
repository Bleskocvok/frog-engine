#pragma once

#include "vector.hpp"
#include "rectangle.hpp"
#include "collision.hpp"
#include "basic.hpp"        // Pi
#include "general.hpp"
#include "polar.hpp"

#include <utility>          // pair
#include <cstddef>          // size_t
#include <stdexcept>        // runtime_error
#include <sstream>          // stringstream
#include <algorithm>        // max, min
#include <vector>

#include <unordered_set>

#include "utils/debug.hpp"

namespace frog::geo
{


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
};


class soft_physics2d
{
public:
    using idx_t = unsigned;

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
        float inv_weight = 1;
    };

    struct settings
    {
        float gravity = 9.81;
        float inertia = 0.98;
        int iterations = 10;
        rect universum = { 0, 0, 1, 1 };
        float delta = 0.016;
        ivec2 grid_dim = { 10, 10 };
    };

private:
    void apply_inertia(point& pt, float delta)
    {
        vec2 diff = pt.pos - pt.prev;
        pt.prev = pt.pos;
        pt.pos += diff * settings_.inertia;
        pt.pos.y() += settings_.gravity * delta * delta;
    }

    void encapsulate(point& pt, rect rect)
    {
        rect.size *= 0.5;
        pt.pos.x() = std::clamp(pt.pos.x(), rect.pos.x() - rect.size.x() + pt.radius,
                                            rect.pos.x() + rect.size.x() - pt.radius);
        pt.pos.y() = std::clamp(pt.pos.y(), rect.pos.y() - rect.size.y() + pt.radius,
                                            rect.pos.y() + rect.size.y() - pt.radius);
    }

    void solve_collision(point& a, point& b)
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

    void solve_joint(joint j)
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

    // TODO
    void solve_angle(angle alpha)
    {
        auto& a = points_[ map_points[ alpha.a ] ].second;
        auto& b = points_[ map_points[ alpha.b ] ].second;
        auto& c = points_[ map_points[ alpha.c ] ].second;

        float cur = vec_angle(a.pos - b.pos, c.pos - b.pos);
        float desired = alpha.angle;
        float dif = desired - cur;

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

    void verlet_solve()
    {
        for (auto& [idx, pt] : points_)
            apply_inertia(pt, settings_.delta);

        // LOGX(points_.size());

        for (int it = 0; it < settings_.iterations; ++it)
        {
            for (auto& [i, pt] : points_)
                encapsulate(pt, settings_.universum);

            auto grid = optimization_grid<std::pair<idx_t, point*>>(
                                        settings_.grid_dim, settings_.universum);
            for (auto& [i, pt] : points_)
                grid.add_to_touching(circle(pt.pos, pt.radius), { i, &pt });

            for (auto& [i, pt] : points_)
                grid.for_each_around(circle(pt.pos, pt.radius), [&](std::pair<idx_t, point*> other)
                    {
                        if (other.first > i)
                            solve_collision(pt, *other.second);
                    });


            // for (int i = 0; i < points_.size(); ++i)
            // {
            //     auto& a = points_[i].second;
            //     encapsulate(a, settings_.universum);
            //     for (int j = i + 1; j < points_.size(); ++j)
            //     {
            //         auto& b = points_[j].second;
            //         solve_collision(a, b);
            //     }
            // }


            for (auto&[idx, j] : joints_)
                solve_joint(j);

            for (auto&[idx, a] : angles_)
                solve_angle(a);
        }
    }

    template<typename K, typename Map>
    static auto& map_at(const std::string& desc, Map& m, const K& key)
    {
        auto it = m.find(key);
        if (it == m.end())
        {
            auto str = std::stringstream{};
            str << desc << ": invalid key '" << key << "'";
            throw std::runtime_error(str.str());
        }
        return it->second;
    }

private:
    idx_t last = 0;
    idx_t updated = 0;

public:
    settings settings_;

    std::vector<std::pair<idx_t, point>> points_;
    std::vector<std::pair<idx_t, joint>> joints_;
    std::vector<std::pair<idx_t, angle>> angles_;

    std::unordered_map<idx_t, std::size_t> map_points;
    std::unordered_map<idx_t, std::size_t> map_joints;

    soft_physics2d(settings s) : settings_(s) {}

    soft_physics2d() = default;

    void update()
    {
        // if (updated != last)
        //     update_maps();
        verlet_solve();
    }

    const auto& point_at(idx_t i) const { return points_[ map_at("soft_physics2d: points", map_points, i) ].second; }
          auto& point_at(idx_t i)       { return points_[ map_at("soft_physics2d: points", map_points, i) ].second; }

    const auto& joint_at(idx_t i) const { return joints_[ map_at("soft_physics2d: joints", map_joints, i) ].second; }
          auto& joint_at(idx_t i)       { return joints_[ map_at("soft_physics2d: joints", map_joints, i) ].second; }

    const auto& points() const { return points_; }
          auto& points()       { return points_; }

    const auto& joints() const { return joints_; }
          auto& joints()       { return joints_; }

    idx_t add_point(point pt)
    {
        map_points.emplace(last, points_.size());
        points_.emplace_back(last, pt);
        return last++;
    }

    idx_t add_joint(joint j)
    {
        map_joints.emplace(last, joints_.size());
        joints_.emplace_back(last, j);
        return last++;
    }

    idx_t add_joint_between(idx_t a, idx_t b)
    {
        return add_joint(joint
        {
            .a = a,
            .b = b,
            .dist = ( point_at(a).pos - point_at(b).pos ).length()
        });
    }

    idx_t add_angle(angle a)
    {
        angles_.emplace_back(last, a);
        return last++;
    }

    void update_maps()
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
};


} // namespace frog::geo
