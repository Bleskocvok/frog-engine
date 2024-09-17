#pragma once

#include "vector.hpp"
#include "matrix.hpp"


namespace frog::geo
{

mat4 perspective(float fov, float ratio, float near, float far);

mat4 translate(vec3 vec);

mat4 scale(vec3 magnitude);

mat2 rotate2d(float rad);

mat4 rotate(int axis, float angle);
mat4 rotate(vec3 axis, float angle);

struct x_axis { int val = 0; };
struct y_axis { int val = 1; };
struct z_axis { int val = 2; };

template<typename T>
mat4 rotate(T axis, float angle)
{
    return rotate(axis.val, angle);
}


} // namespace frog
