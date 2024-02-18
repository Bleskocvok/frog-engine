#include "transform.hpp"

#include "geometry/basic.hpp"

#include <cmath>
#include <cassert>


using frog::geo::mat4;
using frog::geo::mat2;


mat4 frog::geo::perspective(float fov, float ratio, float near, float far)
{
    fov *= geo::ToRad;

    float n = near;                     // near
    float f = far;                      // far
    float t = near * std::tan(fov / 2); // top
    float b = -t;                       // bottom
    float r = t * ratio;                // right
    float l = -r;                       // left

    // make sure this perspective follows the coordinate system
    assert(r > 0);
    assert(l < 0);
    assert(t > 0);
    assert(b < 0);

    return mat4
    {
        2*n/(r-l),          0,              0,  -n*(r+l)/(r-l),
                0,  2*n/(t-b),              0,  -n*(t+b)/(t-b),
                0,          0,   -(f+n)/(f-n),     2*f*n/(n-f),
                0,          0,             -1,               0,
    };
}


mat4 frog::geo::translate(vec3 vec)
{
    auto res = mat4{ 1 };
    for (int i = 0; i < 3; ++i)
    {
        res.at(3, i) = vec[i];
    }
    return res;
}


mat2 frog::geo::rotate2d(float rad)
{
    float cos_theta = std::cos(rad);
    float sin_theta = std::sin(rad);
    return mat2
    {
        cos_theta, -sin_theta,
        sin_theta,  cos_theta,
    };
}


/*
 * I recommend exactly one element of `axis` to
 * be one and all the other to be zero.
 * TODO improve
 */
mat4 frog::geo::rotate(vec3 axis, float angle)
{
    auto res = mat4{ 0 };

    for (int i = 0; i < 3; ++i)
    {
        res += axis[i] * rotate(i, angle);
    }

    return res;
}


mat4 frog::geo::scale(vec3 magnitude)
{
    auto res = mat4{ 1 };
    for (int i = 0; i < 3; ++i)
    {
        res.at(i, i) = magnitude[i];
    }
    return res;
}


// TODO rewrite in a more general way (see wikipedia)
mat4 frog::geo::rotate(int axis, float angle)
{
    float sin = std::sin(angle);
    float cos  = std::cos(angle);

    switch (axis)
    {
        // hooray, copy-paste code!

        // x
        case 0:
            return mat4
            {
                1,    0,   0, 0,
                0,  cos, sin, 0,
                0, -sin, cos, 0,
                0,    0,   0, 1
            };

        // y
        case 1:
            return mat4
            {
                cos, 0, -sin, 0,
                  0, 1,    0, 0,
                sin, 0,  cos, 0,
                  0, 0,    0, 1
            };

        // z
        case 2:
            return mat4
            {
                cos, -sin, 0, 0,
                sin,  cos, 0, 0,
                  0,    0, 1, 0,
                  0,    0, 0, 1
            };

        default:
            assert(false);
        break;
    }

    return mat4{ 1 };
}
