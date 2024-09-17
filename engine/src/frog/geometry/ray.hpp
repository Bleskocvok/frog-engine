#pragma once

#include "vector.hpp"


namespace frog::geo
{


struct ray3
{
    vec3 orig;
    vec3 vec;

    ray3() = default;

    ray3(vec3 origin, vec3 vec)
    : orig(origin), vec(vec) {}

    ray3(float ox, float oy, float oz, float vx, float vy, float vz)
    : orig(ox, oy, oz), vec(vx, vy, vz) {}
};


} // namespace frog::geo

