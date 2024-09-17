#pragma once

#include "rectangle.hpp"
#include "circle.hpp"


namespace frog::geo
{


// rectangle ⨯ rectangle
template<typename T>
bool is_collision(rect_t<T> a, rect_t<T> b)
{
    return a.pos.x() + a.size.x() * 0.5 >= b.pos.x() - b.size.x() * 0.5
        && a.pos.x() - a.size.x() * 0.5 <= b.pos.x() + b.size.x() * 0.5
        && a.pos.y() + a.size.y() * 0.5 >= b.pos.y() - b.size.y() * 0.5
        && a.pos.y() - a.size.y() * 0.5 <= b.pos.y() + b.size.y() * 0.5;
}

// rectangle ⨯ point
template<typename T>
bool is_collision(rect_t<T> a, vec<T, 2> p)
{
    return p.x() >= a.pos.x() - a.size.x() * 0.5
        && p.x() <= a.pos.x() + a.size.x() * 0.5
        && p.y() >= a.pos.y() - a.size.y() * 0.5
        && p.y() <= a.pos.y() + a.size.y() * 0.5;
}

// circle ⨯ point
template<typename T>
bool is_collision(circle_t<T> c, vec<T, 2> p)
{
    return (p - c.pos).length_squared() <= c.radius * c.radius;
}

// rectangle ⨯ circle
template<typename T>
bool is_collision(rect_t<T> a, circle_t<T> c)
{
    auto l = a;  l.size.x() += c.radius * 2;
    auto h = a;  h.size.y() += c.radius * 2;
    return is_collision(l, c.pos)        || is_collision(h, c.pos)
        || is_collision(c, a.top_left()) || is_collision(c, a.top_right())
        || is_collision(c, a.bot_left()) || is_collision(c, a.bot_right());
}

// circle ⨯ rectangle
template<typename T>
bool is_collision(circle_t<T> c, rect_t<T> a) { return is_collision(a, c); }


// point ⨯ rectangle
template<typename T>
bool is_collision(vec<T, 2> p, rect_t<T> a) { return is_collision(a, p); }

// point ⨯ circle
template<typename T>
bool is_collision(vec<T, 2> p, circle_t<T> c) { return is_collision(c, p); }


} // namespace frog::geo
