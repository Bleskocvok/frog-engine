
#include "objects.hpp"

#include "vector.hpp"
#include "matrix.hpp"
#include "general.hpp"

#include <cmath>


bool frog::geo::sphere::collides(sphere other) const
{
    return (other.orig - orig).length_squared() <= std::pow(other.r + r, 2);
}


bool frog::geo::sphere::collides(vec3 pt) const
{
    return (pt - orig).length_squared() <= std::pow(r, 2);
}


// TODO untested, test this
bool frog::geo::aabb::collides(vec3 pt) const
{
    for (int i = 0; i < 3; i++)
    {
        if (orig[i] + size[i] * 0.5f < pt[i])
            return false;

        if (orig[i] - size[i] * 0.5f > pt[i])
            return false;
    }

    return true;
}


bool frog::geo::sphere::collides(aabb o) const
{
    const auto& sph = *this;

    auto d = sph.orig - o.orig;

    for (int i = 0; i < 3; i++)
    {
        d[i] = std::abs(d[i]);

        if (d[i] > o.size[i] * 0.5f + sph.r)
            return false;
    }

    for (int i = 0; i < 3; i++)
    {
        if (d[i] <= o.size[i] * 0.5f)
            return true;
    }

    auto coords = std::initializer_list<std::pair<int, int>>
    {
        { -1, -1 },
        {  1, -1 },
        { -1,  1 },
        {  1,  1 }
    };

    for (int i = 0; i < 3; i++)
    {
        int zero = i;
        int a = (i + 1) % 3;
        int b = (i + 2) % 3;

        for (auto[sgn_a, sgn_b] : coords)
        {
            auto sph2d = sph;
            sph2d.orig[zero] = 0;

            vec3 pt2d = o.orig;
            pt2d[zero] = 0;

            pt2d[a] += sgn_a * o.size[a] * 0.5f;
            pt2d[b] += sgn_b * o.size[b] * 0.5f;

            if (sgn_a * sph2d.orig[a] > sgn_a * pt2d[a]
                    && sgn_b * sph2d.orig[b] > sgn_b * pt2d[b])
                continue;

            if (!sph2d.collides(pt2d))
                return false;
        }
    }

    return true;
}


bool frog::geo::aabb::collides(sphere other) const
{
    return other.collides(*this);
}


bool frog::geo::aabb::collides(aabb o) const
{
    for (int i = 0; i < 3; i++)
    {
        if (orig[i] + size[i] * 0.5f < o.orig[i] - o.size[i] * 0.5f)
            return false;

        if (orig[i] - size[i] * 0.5f > o.orig[i] + o.size[i] * 0.5f)
            return false;
    }

    return true;
}
