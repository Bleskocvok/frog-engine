#pragma once

#include "objects.hpp"
#include "vector.hpp"
#include "polar.hpp"

#include <optional>


namespace frog::geo
{


struct paddle
{
    float angular_size = 15;
    float thickness = 1;
    float height = 1;

    cylindrical3 pos{ 1, 0, 0 };
};


struct collision_info
{
    //enum class type { Inner, Outer, Edge, Side };
    //type ctype;

    geo::vec3 normal;
    geo::vec3 resolved;
};


std::optional<collision_info> collision_2D(paddle, cylindrical3, float radius);


} // namespace frog::geo

