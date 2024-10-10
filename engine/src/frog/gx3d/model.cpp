#ifndef NOT_FROG_BUILD_3D

#include "model.hpp"

#include "mesh.hpp"
#include "program.hpp"
#include "material.hpp"
#include "frog/geometry/transform.hpp"

#include <cassert>


using namespace frog::geo;


mat4 frog::gl::model::matrix() const
{
    mat4 res{ 1 };

    // 1. scale
    res = geo::scale(scale) * res;

    // 2. rotate
    for (int axis : { 2, 0, 1 })    // z, x, y
    {
        res = rotate(axis, rot[axis]) * res;
    }

    // 3. translate
    return translate(pos) * res;
}

#endif
