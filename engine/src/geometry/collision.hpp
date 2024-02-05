#pragma once

#include "rectangle.hpp"


namespace frog::geo
{


template<typename T>
bool is_collision(rect_t<T> a, rect_t<T> b)
{
    return a.pos.x() + a.size.x() * 0.5 >= b.pos.x() - b.size.x() * 0.5
        && a.pos.x() - a.size.x() * 0.5 <= b.pos.x() + b.size.x() * 0.5
        && a.pos.y() + a.size.y() * 0.5 >= b.pos.y() - b.size.y() * 0.5
        && a.pos.y() - a.size.y() * 0.5 <= b.pos.y() + b.size.y() * 0.5;
}


template<typename T>
bool is_collision(rect_t<T> a, vec<T, 2> p)
{
    return p.x() >= a.pos.x() - a.size.x() * 0.5
        && p.x() <= a.pos.x() + a.size.x() * 0.5
        && p.y() >= a.pos.y() - a.size.y() * 0.5
        && p.y() <= a.pos.y() + a.size.y() * 0.5;
}


template<typename T>
bool is_collision(vec<T, 2> p, rect_t<T> a)
{
    return is_collision(a, p);
}


} // namespace frog::geo
