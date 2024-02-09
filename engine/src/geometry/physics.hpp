#pragma once

#include "vector.hpp"
#include "rectangle.hpp"
#include "basic.hpp"        // Pi

#include <utility>          // pair
#include <vector>


namespace frog::geo
{


class soft_physics2d
{
public:
    using idx_t = unsigned;

    // // collection of points, joints between them and angles
    // struct collection {};

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
        pt.pos.x() = std::clamp(pt.pos.x(), rect.pos.x() - rect.size.x(),
                                            rect.pos.x() + rect.size.x());
        pt.pos.y() = std::clamp(pt.pos.y(), rect.pos.y() - rect.size.y(),
                                            rect.pos.y() + rect.size.y());
    }

    void solve_collision(point& a, point& b)
    {
        vec2 dif = a.pos - b.pos;
        float dist = dif.length();
        float radii = a.radius + b.radius;

        if (dist < radii)
        {
            float ratio = (dist - radii) / (dist * (a.inv_weight * b.inv_weight));
            a.pos -= dif * (ratio * a.inv_weight * 0.5);
            b.pos += dif * (ratio * b.inv_weight * 0.5);
        }
    }

    void verlet_solve(float delta)
    {
        for (auto& [idx, pt] : points_)
            apply_inertia(pt, delta);

        for (int it = 0; it < settings_.iterations; ++it)
        {
            for (int i = 0; i < points_.size(); ++i)
            {
                auto& a = points_[i].second;
                encapsulate(a, settings_.universum);
                for (int j = i + 1; j < points_.size(); ++j)
                {
                    auto& b = points_[j].second;
                    solve_collision(a, b);
                }
            }
        }
    }

private:
    idx_t last = 0;

public:
    settings settings_;

    std::vector<std::pair<idx_t, point>> points_;
    std::vector<std::pair<idx_t, point>> joints_;

    soft_physics2d(settings s) : settings_(s) {}

    soft_physics2d() = default;

    void update()
    {
        verlet_solve(settings_.delta);
    }

    const auto& points() const { return points_; }
          auto& points()       { return points_; }

    const auto& joints() const { return joints_; }
          auto& joints()       { return joints_; }

    idx_t add_point(point pt)
    {
        points_.emplace_back(last, pt);
        return last++;
    }

    void add_joint(idx_t col, joint j)
    { }
};


} // namespace frog::geo
