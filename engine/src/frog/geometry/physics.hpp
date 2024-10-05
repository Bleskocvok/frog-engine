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

#include "frog/utils/debug.hpp"


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
        ivec2 grid_dim = { 10, 10 };
    };

private:
    void apply_inertia(point& pt, float delta);

    void encapsulate(point& pt, rect rect);

    void solve_collision(point& a, point& b);

    void solve_joint(joint j);

    void solve_angle(angle alpha);

    void verlet_solve();

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

    const auto& settings() const { return settings_; }
          auto& settings()       { return settings_; }

    void update()
    {
        // TODO: ??? Consider deletion???
        if (updated != last)
            update_maps();

        verlet_solve();
    }

    void update_maps();

    const point& point_at(idx_t i) const { return points_[ map_at("soft_physics2d: points", map_points, i) ].second; }
          point& point_at(idx_t i)       { return points_[ map_at("soft_physics2d: points", map_points, i) ].second; }

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

    // TODO: Implement deletion.
    void delete_point(idx_t i) {}
    void delete_joint(idx_t i) {}
    void delete_angle(idx_t i) {}
};


} // namespace frog::geo
