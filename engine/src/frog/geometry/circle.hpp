#pragma once

#include "frog/geometry/vector.hpp"


namespace frog::geo
{


template<typename T>
struct circle_t;

using circle = circle_t<float>;
using icircle = circle_t<int>;


template<typename T>
struct circle_t
{
    // the centre point
    vec<T, 2> pos;
    T radius;

    circle_t() = default;

    circle_t(vec<T, 2> mid, T r) : pos(mid), radius(r) {}
    circle_t(T x, T y, T r) : pos(x, y), radius(r) {}
};


}  // namespace frog::geo
