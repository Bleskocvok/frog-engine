#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "geometry/vector.hpp"


namespace frog::gl
{
    class mesh;
}


namespace frog::gl::shape
{


mesh cuboid(frog::geo::vec3 size);

mesh sphere(int rings, int sides, float radius);

mesh polygon_2D(int sides, float radius);

mesh paddle(int sides,
            float radius,
            float thickness,
            float height,
            float theta);


} // namespace frog::gl::shape

#endif
