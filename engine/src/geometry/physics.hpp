#pragma once

#include "vector.hpp"
#include "rectangle.hpp"
#include "basic.hpp"        // Pi

#include <utility>          // pair
#include <cstddef>          // size_t
#include <stdexcept>        // runtime_error
#include <sstream>          // stringstream
#include <vector>


namespace frog::geo
{


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

    // void solve_angle(angle alpha) { }

    void verlet_solve()
    {
        for (auto& [idx, pt] : points_)
            apply_inertia(pt, settings_.delta);

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

            for (auto&[idx, j] : joints_)
                solve_joint(j);
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

    std::unordered_map<idx_t, std::size_t> map_points;
    std::unordered_map<idx_t, std::size_t> map_joints;

    soft_physics2d(settings s) : settings_(s) {}

    soft_physics2d() = default;

    void update()
    {
        if (updated != last)
            update_maps();
        verlet_solve();
    }

    const auto& point_at(idx_t i) const { return points_[ map_at("soft_physics2d: points", map_points, i) ]; }
          auto& point_at(idx_t i)       { return points_[ map_at("soft_physics2d: points", map_points, i) ]; }

    const auto& joint_at(idx_t i) const { return joints_[ map_at("soft_physics2d: joints", map_joints, i) ]; }
          auto& joint_at(idx_t i)       { return joints_[ map_at("soft_physics2d: joints", map_joints, i) ]; }

    const auto& points() const { return points_; }
          auto& points()       { return points_; }

    const auto& joints() const { return joints_; }
          auto& joints()       { return joints_; }

    idx_t add_point(point pt)
    {
        points_.emplace_back(last, pt);
        return last++;
    }

    idx_t add_joint(joint j)
    {
        joints_.emplace_back(last, j);
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
