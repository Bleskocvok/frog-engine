#pragma once

#include "vector.hpp"

#include <cmath>
#include <vector>
#include <utility>      // std::pair
#include <optional>


namespace frog::geo
{


inline float canonize(float angle)
{
    return std::fmod(angle + Pi / 2, 2 * Pi) - Pi / 2;
}

inline float angle_diff(float a, float b)
{
    float res = canonize(b) - canonize(a) + 2 * Pi;
    res = std::fmod(res + Pi / 2, 2 * Pi) - Pi / 2;
    return res;
}


inline vec2 to_vec2(vec3 v) { return { v.x(), v.z() }; }


inline vec3 to_vec3(vec2 v) { return { v.x(), 0, v.y() }; }


struct ray3;


std::optional<vec3> intersection(ray3 a, ray3 b);


vec3 reflect(vec3 vector, vec3 normal);


/** Distance calculations
 */

vec3 closest_point(ray3 ray, vec3 point);

float distance(ray3 a, ray3 b);

float distance(vec3 a, vec3 b);

float distance(ray3 a, vec3 point);

float distance(vec3 point, ray3 a);


template<typename Func>
std::vector<std::pair<float, float>> ode_solutions(
        Func func, float x0, float y0, float h, float xn)
{
    auto result = std::vector<std::pair<float, float>>{};

    auto add = [&](float x, float y)
    {
        result.emplace_back(x, y);
    };

    euler_steps(func, x0, y0, h, xn, add);

    return result;
}


template<typename Func>
std::pair<float, float> ode_solve(
        Func func, float x0, float y0, float h, float xn)
{
    float res_x = 0,
          res_y = 0;

    auto last = [&](float x, float y)
    {
        res_x = x;
        res_y = y;
    };

    euler_steps(func, x0, y0, h, xn, last);

    return { res_x, res_y };
}


template<typename InputFunc, typename CollectFunc>
void euler_steps(InputFunc func
                , float x0
                , float y0
                , float h
                , float xn
                , CollectFunc collect)
{
    collect(x0, y0);

    int n = (xn - x0) / h;

    float x = x0,
          y = y0;

    for (int i = 0; i < n; i++)
    {
        y = y0 + h * func(x0, y0);
        x += h;

        // maybe an if statement whether x < xn
        x0 = x;
        y0 = y;

        collect(x0, y0);
    }
}


} // namespace frog::geo
