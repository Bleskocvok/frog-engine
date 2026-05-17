#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/animation.hpp"
#include "frog/utils/assert.hpp"

#include <algorithm>
#include <cmath>        // lerp
#include <cstdint>
#include <iterator>
#include <list>
#include <map>
#include <tuple>        // get
#include <utility>      // move, forward
#include <variant>
#include <vector>

namespace frog::scripts {

enum class InterFunc
{
    Lerp,
    Smoothstep,
    Smootherstep
};

template<typename T>
T interpolate(InterFunc interf, T a, T b, double t)
{
    switch (interf)
    {
        case InterFunc::Lerp:         return frog::geo::lerp_t(a, b, t);
        case InterFunc::Smoothstep:   return frog::geo::smoothstep(a, b, t);
        case InterFunc::Smootherstep: return frog::geo::smootherstep(a, b, t);
    }
    frog_assert(false);
}

struct Scale
{
    frog::geo::vec2 scale = { 1 };

    InterFunc interfunc = InterFunc::Smoothstep;

    friend auto& operator<<(auto& o, Scale s)
    {
        return o << s.scale;
    }
};

struct Position
{
    frog::geo::vec2 delta = { 0 };

    // TODO: Relative to scale?
    // bool relative = true;

    InterFunc interfunc = InterFunc::Smoothstep;
};

struct Rotation
{
    double deg = 0;
    InterFunc interfunc = InterFunc::Smoothstep;
};

struct Color
{
    frog::gx::rgba_t color;
};

struct Loop {};

struct End {};


namespace detail {

template<typename T>
struct Node
{
    uint64_t t = 0;
    T key;

    friend auto& operator<<(auto& o, Node n)
    {
        return o << "Node{ " << n.t << ", " << n.key << " }";
    }
};

template<typename T>
struct Timeline
{
    std::map<uint64_t, Node<T>> data;

    T accum;

    uint64_t add(uint64_t t, T key)
    {
        Node<T> node;
        node.t = t;
        node.key = std::move(key);
        data.emplace(t, std::move(node));
        return t;
    }

    void remove(uint64_t t)
    {
        data.erase(t);
    }

    std::pair<Node<T>*, Node<T>*> prev_next(uint64_t t)
    {
        if (data.empty())
            return { nullptr, nullptr };

        auto upper = data.upper_bound(t);
        if (upper == data.end())
            return { &data.rbegin()->second, nullptr };

        if (data.begin() == upper)
            return { nullptr, &upper->second };

        return { &std::prev(upper)->second, &upper->second };
    }
};

} // namespace detail

class Keyframes : public frog::script2d
{
    gx2d::Sprite* sprite = nullptr;

    template<typename... Ts>
    struct Timelines
    {
        std::tuple< detail::Timeline<Ts>... > data;

        template<typename T>
        detail::Timeline<T>& get()
        { return std::get<detail::Timeline<T>>(data); }

        template<typename T>
        const detail::Timeline<T>& get() const
        { return std::get<detail::Timeline<T>>(data); }
    };

    Timelines<Scale, Rotation, Position, Color, Loop, End> timelines;

    uint64_t accum = 0;

    template<typename T>
    void solve(double, const detail::Node<T>&, detail::Node<T>&)
    {
        static_assert(false);
    }

    template<>
    void solve<Position>(double between,
            const detail::Node<Position>& prev,
            detail::Node<Position>& next)
    {
        frog_assert(sprite);

        auto& accum = timelines.get<Position>().accum;

        sprite->rect.pos -= accum.delta;

        auto delta = interpolate(next.key.interfunc, prev.key.delta, next.key.delta, between);
        accum.delta = delta;

        sprite->rect.pos += delta;
    }

    template<>
    void solve<Scale>(double between, const detail::Node<Scale>& prev, detail::Node<Scale>& next)
    {
        frog_assert(sprite);

        auto& accum = timelines.get<Scale>().accum;

        if (accum.scale.x() != 0)
            sprite->rect.size.x() /= accum.scale.x();

        if (accum.scale.y() != 0)
            sprite->rect.size.y() /= accum.scale.y();

        auto scale = interpolate(next.key.interfunc, prev.key.scale, next.key.scale, between);
        accum.scale = scale;

        sprite->rect.size *= scale;
    }

    template<>
    void solve<Rotation>(double between, const detail::Node<Rotation>& prev, detail::Node<Rotation>& next)
    {
        frog_assert(sprite);

        auto& accum = timelines.get<Rotation>().accum;

        sprite->angle -= accum.deg;

        auto deg = interpolate(next.key.interfunc, prev.key.deg, next.key.deg, between);
        accum.deg = deg;

        sprite->angle += deg;
    }

    template<typename T>
    void solve_transition()
    {
        auto [prev, next] = timelines.get<T>().prev_next(accum);

        if (not next)
            return;

        detail::Node<T> def;
        if (not prev)
            prev = &def;

        uint64_t range = next->t - prev->t;

        double between = (accum - prev->t) / double(range);
        between = std::clamp(between, 0.0, 1.0);

        solve<T>(between, *prev, *next);
    }

    template<typename T>
    bool reached()
    {
        auto [prev, next] = timelines.get<T>().prev_next(accum);
        return prev != nullptr || ( next != nullptr && accum >= next->t );
    }

public:
    Keyframes(gx2d::Sprite& sprite)
        : sprite(&sprite)
    { }

    void frame_update(frog::game_object2d&, frog::engine2d& eng) override
    {
        accum += eng.global->frame_time() * 1000;

        solve_transition<Scale>();
        solve_transition<Rotation>();
        solve_transition<Position>();
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d&) override
    {
        if (reached<End>())
            obj.remove_script(this);

        // TODO: Accum -= Loop.t
        if (reached<Loop>())
            accum = 0;
    }

    template<typename T>
    uint64_t add(uint64_t t, T val)
    {
        return timelines.get<T>().add(t, std::move(val));
    }
};


} // namespace frog
