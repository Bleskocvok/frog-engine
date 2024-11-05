#pragma once

#include "vector.hpp"


namespace frog::geo
{


template<typename T>
struct rect_t;

using rect = rect_t<float>;
using irect = rect_t<int>;


template<typename T>
struct rect_t
{
    // the centre point
    vec<T, 2> pos;
    vec<T, 2> size = { 1, 1 };

    rect_t() = default;

    rect_t(vec<T, 2> mid, vec<T, 2> size) : pos(mid), size(size) {}
    rect_t(T x, T y, T w, T h) : pos(x, y), size(w, h) {}

    vec<T, 2> top_left()  const { return pos - size * 0.5; }
    vec<T, 2> top_right() const { return pos + 0.5 * vec<T, 2>{ +size.x(), -size.y() }; }
    vec<T, 2> bot_left()  const { return pos + 0.5 * vec<T, 2>{ -size.x(), +size.y() }; }
    vec<T, 2> bot_right() const { return pos + 0.5 * vec<T, 2>{ +size.x(), +size.y() }; }
};


} // namespace frog::geo
