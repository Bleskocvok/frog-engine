#pragma once

#include "primitives.hpp"
#include "optimization_grid.hpp"
#include "collisions.hpp"
#include "container.hpp"
#include "idx.hpp"

#include "frog/geometry/vector.hpp"
#include "frog/geometry/rectangle.hpp"

#include <ios>
#include <memory>
#include <optional>
#include <unordered_set>
#include <utility>          // move, pair, forward
#include <stdexcept>        // runtime_error
#include <sstream>          // stringstream
#include <vector>           // vector

namespace frog::geo {

class soft_physics2d
{
public:
    using point = Point;
    using joint = Joint;
    using angle = Angle;
    using idx_t = ::frog::geo::idx_t;

    class iplugin
    {
    public:
        virtual void solve_point(point&, idx_t) {}

        virtual void before() {}
        virtual void after() {}

        virtual void solve_all([[maybe_unused]] int iter,
                               std::vector<std::pair<idx_t, point>>& points)
        {
            for (auto& [i, pt] : points)
                solve_point(pt, i);
        }

        virtual ~iplugin() = default;
    };

    struct Settings
    {
        vec2 gravity = { 0, 9.81 };
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

    // struct CollisionInfo
    // {
    //     idx_t a = -1;
    //     idx_t b = -1;
    //     bool includes(idx_t i) const
    //     {
    //         return a == i || b == i;
    //     }
    // };

    Collisions<std::unordered_set> collisions_;

private:
    Settings settings_;
    limits limits_ = {};

    rect prev_universum;

    Container<point> points_;
    Container<joint> joints_;
    Container<angle> angles_;

    std::vector<idx_t> points_removal;
    std::vector<idx_t> joints_removal;
    std::vector<idx_t> angles_removal;

    std::vector<std::unique_ptr<iplugin>> plugins_;

    std::optional<optimization_grid<std::pair<idx_t, point*>>> grid;

    void apply_inertia(point& pt, float delta);

    bool encapsulate(point& pt, rect rect);

    void solve_joint(joint j);

    void solve_angle(angle alpha);

    void verlet_solve();

    void remove();

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

    void calculate_grid();

public:
    soft_physics2d(Settings s, limits l = limits{})
        : settings_(s), limits_(l), points_(l.points), joints_(l.joints)
        , angles_(l.angles)
    { }

    soft_physics2d(limits l = limits{}) : soft_physics2d({}, l) {}

    void add_plugin(std::unique_ptr<iplugin> plugin)
    {
        plugins_.push_back(std::move(plugin));
    }

    const auto& plugins() const { return plugins_; }

    const auto& settings() const { return settings_; }
          auto& settings()       { return settings_; }

    static bool solve_collision(point& a, point& b);

    const auto& collisions() const { return collisions_; }

    void update()
    {
        if (prev_universum != settings().universum)
            grid.reset();

        for (auto& [idx, pt] : points_.data)
        {
            if (collisions_.contains(collisions_.all_without_joints_, idx))
                pt.flying = 0;
            else
                pt.flying++;
        }

        // recorder.clear();

        collisions_.reset();

        remove();
        verlet_solve();
        prev_universum = settings().universum;


        // if (recorder.size() >= 3000)
        // {
        //     {
        //         std::stringstream o;
        //         o << "{\n";
        //         for (auto info : recorder)
        //             o << std::fixed << "{" << info.first << "," << info.second << "},\n";

        //         o << "}\n";
        //         frog::lib2d::fs::file_write("record_collisions.txt", o.str(), lib2d::fs::mode::Binary);
        //     }

        //     {
        //         std::stringstream o;
        //         o << "{\n";

        //         for (auto j : joints_.data)
        //         {
        //                 o << "{" << j.first << ","
        //                   << "Joint{.a=" << j.second.a << ",.b=" << j.second.b << ",.dist=" << std::fixed << j.second.dist << "}},\n";

        //         }
        //         o << "}\n";
        //         frog::lib2d::fs::file_write("record_joints.txt", o.str(), lib2d::fs::mode::Binary);
        //     }
        // }

        collisions_.update(joints_);
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

    // TODO: Add limit_reached check.
    idx_t add_point(point pt) { return points_.push(pt); }
    idx_t add_joint(joint j) { return joints_.push(j); }
    idx_t add_angle(angle a) { return angles_.push(a); }

    idx_t add_joint_between(idx_t a, idx_t b)
    {
        return add_joint(joint
        {
            .a = a,
            .b = b,
            .dist = ( point_at(a).pos - point_at(b).pos ).length()
        });
    }

    void remove_point(idx_t i) { points_removal.push_back(i); }
    void remove_joint(idx_t i) { joints_removal.push_back(i); }
    void remove_angle(idx_t i) { angles_removal.push_back(i); }

    void push(idx_t point, vec2 delta);

    template<typename Func>
    void for_each_colliding(circle c, Func&& func)
    {
        if (not grid)
            calculate_grid();

        auto visited = std::unordered_set<idx_t>{};

        grid->for_each_around(c, [&](std::pair<idx_t, point*> pt)
            {
                if (visited.contains(pt.first))
                    return;
                visited.insert(pt.first);
                func(pt.first, pt.second);
            });
    }

    void explode(circle c, float power);
};


} // namespace frog::geo
