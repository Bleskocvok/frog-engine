#pragma once

#include "vector.hpp"


namespace frog::geo
{


struct aabb;
struct sphere;


struct sphere
{
    vec3 orig;
    float r = 1;

    sphere() = default;

    sphere(vec3 origin, float radius) : orig(origin), r(radius) {}

    sphere(float x, float y, float z, float radius)
        : orig(x, y, z), r(radius) {}

    bool collides(sphere other) const;
    bool collides(aabb other) const;
    bool collides(vec3 pt) const;
};


// axis aligned bounding box
struct aabb
{
    vec3 orig;
    vec3 size{ 1 };

    aabb(vec3 origin, vec3 size) : orig(origin), size(size) {}

    aabb(float x, float y, float z, float sx, float sy, float sz)
        : orig(x, y, z), size(sx, sy, sz) {}

    aabb() = default;

    bool collides(sphere other) const;
    bool collides(aabb other) const;
    bool collides(vec3 pt) const;
};


} // namespace frog::geo


