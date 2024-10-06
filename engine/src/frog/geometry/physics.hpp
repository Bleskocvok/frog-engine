#pragma once

#include "vector.hpp"
#include "rectangle.hpp"
#include "collision.hpp"
#include "basic.hpp"        // Pi
#include "general.hpp"
#include "polar.hpp"

#include <utility>          // move, pair, forward
#include <cstddef>          // size_t
#include <stdexcept>        // runtime_error
#include <sstream>          // stringstream
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
        // My past self was probably thinking optimization, using radians is
        // faster (sin/cos use them, so no need for conversion).
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

    struct limits
    {
        unsigned points = 10000;
        unsigned joints = 10000;
        unsigned angles = 10000;

        // This is for some reason needed by ‹limits l = limits{}› in the
        // constructors. ‹= default› doesn't help here either.
        limits() {}

        // Omg, now I need to specify the full constructor.
        limits(unsigned points, unsigned joints, unsigned angles)
            : points(points)
            , joints(joints)
            , angles(angles) {}
    };

    // All of this to allow for deletion, because index reuse becomes quite
    // necessary.
    template<typename T>
    struct container
    {
        idx_t limit_;
        std::vector<std::pair<idx_t, T>> data;
        std::vector<idx_t> free;
        std::vector<int> map;
        idx_t last = 0;

        explicit container(int limit) : limit_(limit), map(limit, -1)
        {
            data.reserve(limit);
        }

        template<typename... Args>
        idx_t emplace(Args&&... args)
        {
            if (free.empty() && last >= limit_)
                throw std::runtime_error("physics container emplace: limit reached");

            idx_t i;

            if (!free.empty())
            {
                i = free.back();
                free.pop_back();
            }
            else
                i = data.size();

            data.emplace_back(i, T( std::forward<Args>(args)... ));
            map[i] = data.size() - 1;
            return i;
        }

        idx_t push(T val)
        {
            return emplace(std::move(val));
        }

        // TODO: Add a remove(multiple) method to be more efficient when
        // removing multiple (using this thing it would be quadratic).
        void remove(idx_t i)
        {
            if (i >= limit_)
                throw std::runtime_error("physics container: invalid index");
            if (map[i] == -1)
                throw std::runtime_error("physics container: cannot delete already deleted element");

            auto removed = map[i];
            data.erase(data.begin() + removed);
            map[i] = -1;
            free.push_back(i);

            for (auto& idx : map)
            {
                if (idx > removed)
                    --idx;
            }
        }

        std::size_t size() const { return data.size(); }

        const T& at(idx_t i) const { return data[ map[ i ] ].second; }
              T& at(idx_t i)       { return data[ map[ i ] ].second; }
    };

private:
    settings settings_;
    limits limits_ = {};

    container<point> points_;
    container<joint> joints_;
    container<angle> angles_;

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

public:
    soft_physics2d(settings s, limits l = limits{})
        : settings_(s), limits_(l), points_(l.points), joints_(l.joints)
        , angles_(l.angles)
    { }

    soft_physics2d(limits l = limits{}) : soft_physics2d({}, l) {}

    const auto& settings() const { return settings_; }
          auto& settings()       { return settings_; }

    void update()
    {
        verlet_solve();
    }

    bool limit_reached() const
    {
        return points_.size() >= limits_.points || joints_.size() >= limits_.joints
            || angles_.size() >= limits_.angles;
    }

    const point& point_at(idx_t i) const { return points_.at(i); }
          point& point_at(idx_t i)       { return points_.at(i); }

    const auto& joint_at(idx_t i) const { return joints_.at(i); }
          auto& joint_at(idx_t i)       { return joints_.at(i); }

    const auto& points() const { return points_.data; }
          auto& points()       { return points_.data; }

    const auto& joints() const { return joints_.data; }
          auto& joints()       { return joints_.data; }

    const auto& angles() const { return angles_.data; }
          auto& angles()       { return angles_.data; }

    idx_t add_point(point pt)
    {
        return points_.push(pt);
    }

    idx_t add_joint(joint j)
    {
        return joints_.push(j);
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
        return angles_.push(a);
    }

    void remove_point(idx_t i)
    {
        points_.remove(i);

        // TODO: Burn this awful thing!
        for (int j = joints_.size() - 1; j >= 0; --j)
        {
            if (joints_.data[j].second.a == i
             || joints_.data[j].second.b == i)
                joints_.remove(joints_.data[j].first);
        }
    }

    void remove_joint(idx_t i)
    {
        joints_.remove(i);
    }

    void remove_angle(idx_t i)
    {
        angles_.remove(i);
    }
};


} // namespace frog::geo
