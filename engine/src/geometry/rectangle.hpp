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

    vec<T, 2> top_left() const { return pos - size * 0.5; }
};


} // namespace frog::geo
