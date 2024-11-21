
#include "general.hpp"

#include "vector.hpp"
#include "ray.hpp"

#include <cassert>


using namespace frog::geo;


std::optional<vec3> frog::geo::intersection(ray3 r1, ray3 r2)
{
    vec3 a = r1.vec,
         b = r2.vec,
         c = r2.orig - r1.orig;

    auto cross = a.cross(b);

    if (cross == vec3{ 0 })
    {
        return is_close(distance(r1.orig, r2), 0)
                ? std::optional{ r1.orig }
                : std::nullopt;
    }

    assert(cross.length_squared() != 0);

    vec3 res = r1.orig + a.dot(c.cross(b).dot(cross)) / cross.length_squared();

    return { res };
}


vec3 frog::geo::reflect(vec3 vector, vec3 normal)
{
    // TODO: Add debug assert that normal is normalized

    vec3 n = normal;
    vec3 v = vector.normalized();
    return (v - 2 * n.dot(v) * n) * vector.length() / n.length_squared();
}


vec3 frog::geo::closest_point(ray3 ray, vec3 point)
{
    float len2 = ray.vec.length_squared();

    if (len2 == 0)
        return point;

    vec3 p_to_r = point - ray.orig;

    float dist = p_to_r.dot(ray.vec) / len2;

    auto res = ray.orig + ray.vec * dist;

    assert(distance(res, point) == distance(point, ray));

    return res;
}


float frog::geo::distance(ray3 r1, ray3 r2)
{
    vec3 a = r1.vec,
         b = r2.vec,
         c = r2.orig - r1.orig;

    vec3 cross = a.cross(b);

    if (cross == vec3(0))
        return 0;

    float dist = std::abs(c.dot(cross)) / cross.length();

    return dist;
}


float frog::geo::distance(vec3 a, vec3 b)
{
    return (a - b).length();
}


float frog::geo::distance(ray3 a, const vec3 point)
{
    return distance(point, a);
}

float frog::geo::distance(vec3 point, ray3 ray)
{
    vec3 x0 = point,
         x1 = ray.orig,
         x2 = ray.orig + ray.vec;

    vec3 cross = ((x0 - x1).cross(x0 - x2));

    if (cross == vec3(0))
        return 0;

    assert((x2 - x1).length() != 0);

    float dist = cross.length() / ray.vec.length();

    return dist;
}
