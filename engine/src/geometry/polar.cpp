#include "polar.hpp"

#include <cmath>


using namespace frog::geo;


polar2 frog::geo::to_polar(vec2 pt)
{
    auto res = polar2{};
    res.r =      std::sqrt(std::pow(pt.x(), 2) + std::pow(pt.y(), 2));
    res.theta =  std::atan2(pt.y(), pt.x());

    return res;
}


vec2 frog::geo::to_cartesian(polar2 pt)
{
    return vec2
    {
        pt.r * std::cos(pt.theta),
        pt.r * std::sin(pt.theta)
    };
}


cylindrical3 frog::geo::to_cylindrical(vec3 pt)
{
    auto pol = to_polar(vec2{ pt.x(), pt.z() });
    return cylindrical3 { pol.r, pol.theta, pt.y() };
}


vec3 frog::geo::to_cartesian(cylindrical3 cyl)
{
    vec2 pt = to_cartesian(polar2{ cyl.r, cyl.theta });
    return vec3{ pt.x(), cyl.y, pt.y() };
}
