#pragma once

#include "vector.hpp"


namespace frog::geo
{


struct polar2
{
    float r = 0;
    float theta = 0;    // in radians
};


struct cylindrical3
{
    float r = 0;
    float theta = 0;    // in radians
    float y = 0;
};


/** 2D cartesian <-> 2D polar
 */

polar2 to_polar(vec2 pt);
vec2 to_cartesian(polar2 pt);


/** 3D cartesian <-> 3D cylindrical
 */

cylindrical3 to_cylindrical(vec3 pt);
vec3 to_cartesian(cylindrical3 pt);


inline polar2 to_polar(cylindrical3 cyl) { return { cyl.r, cyl.theta }; }

inline cylindrical3 to_cylindrical(polar2 pol)
{
    return { pol.r, pol.theta, 0 };
}


} // namespace frog::geo
